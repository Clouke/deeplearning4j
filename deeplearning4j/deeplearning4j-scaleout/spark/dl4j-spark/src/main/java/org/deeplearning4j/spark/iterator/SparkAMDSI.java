/*
 *  ******************************************************************************
 *  *
 *  *
 *  * This program and the accompanying materials are made available under the
 *  * terms of the Apache License, Version 2.0 which is available at
 *  * https://www.apache.org/licenses/LICENSE-2.0.
 *  *
 *  *  See the NOTICE file distributed with this work for additional
 *  *  information regarding copyright ownership.
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  * License for the specific language governing permissions and limitations
 *  * under the License.
 *  *
 *  * SPDX-License-Identifier: Apache-2.0
 *  *****************************************************************************
 */

package org.deeplearning4j.spark.iterator;

import lombok.NonNull;
import lombok.extern.slf4j.Slf4j;
import org.apache.spark.TaskContext;
import org.nd4j.linalg.dataset.AsyncMultiDataSetIterator;
import org.nd4j.linalg.dataset.api.MultiDataSet;
import org.nd4j.linalg.dataset.api.iterator.MultiDataSetIterator;
import org.nd4j.linalg.dataset.callbacks.DataSetCallback;
import org.nd4j.linalg.dataset.callbacks.DefaultCallback;
import org.nd4j.linalg.factory.Nd4j;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

@Slf4j
public class SparkAMDSI extends AsyncMultiDataSetIterator {
    protected TaskContext context;

    protected SparkAMDSI() {
        super();
    }

    public SparkAMDSI(MultiDataSetIterator baseIterator) {
        this(baseIterator, 8);
    }

    public SparkAMDSI(MultiDataSetIterator iterator, int queueSize, BlockingQueue<MultiDataSet> queue) {
        this(iterator, queueSize, queue, true);
    }

    public SparkAMDSI(MultiDataSetIterator baseIterator, int queueSize) {
        this(baseIterator, queueSize, new LinkedBlockingQueue<MultiDataSet>(queueSize));
    }

    public SparkAMDSI(MultiDataSetIterator baseIterator, int queueSize, boolean useWorkspace) {
        this(baseIterator, queueSize, new LinkedBlockingQueue<MultiDataSet>(queueSize), useWorkspace);
    }

    public SparkAMDSI(MultiDataSetIterator baseIterator, int queueSize, boolean useWorkspace, Integer deviceId) {
        this(baseIterator, queueSize, new LinkedBlockingQueue<MultiDataSet>(queueSize), useWorkspace,
                        new DefaultCallback(), deviceId);
    }

    public SparkAMDSI(MultiDataSetIterator baseIterator, int queueSize, boolean useWorkspace,
                    DataSetCallback callback) {
        this(baseIterator, queueSize, new LinkedBlockingQueue<MultiDataSet>(queueSize), useWorkspace, callback);
    }

    public SparkAMDSI(MultiDataSetIterator iterator, int queueSize, BlockingQueue<MultiDataSet> queue,
                    boolean useWorkspace) {
        this(iterator, queueSize, queue, useWorkspace, null);
    }

    public SparkAMDSI(MultiDataSetIterator iterator, int queueSize, BlockingQueue<MultiDataSet> queue,
                    boolean useWorkspace, DataSetCallback callback) {
        this(iterator, queueSize, queue, useWorkspace, callback, Nd4j.getAffinityManager().getDeviceForCurrentThread());
    }

    public SparkAMDSI(MultiDataSetIterator iterator, int queueSize, BlockingQueue<MultiDataSet> queue,
                    boolean useWorkspace, DataSetCallback callback, Integer deviceId) {
        this();

        if (queueSize < 2)
            queueSize = 2;

        this.callback = callback;
        this.buffer = queue;
        this.backedIterator = iterator;
        this.useWorkspaces = useWorkspace;
        this.prefetchSize = queueSize;
        this.workspaceId = "SAMDSI_ITER-" + java.util.UUID.randomUUID().toString();
        this.deviceId = deviceId;

        if (iterator.resetSupported())
            this.backedIterator.reset();

        this.thread = new SparkPrefetchThread(buffer, iterator, terminator, Nd4j.getAffinityManager().getDeviceForCurrentThread());

        context = TaskContext.get();

        thread.setDaemon(true);
        thread.start();
    }

    @Override
    protected void externalCall() {
    }

    protected class SparkPrefetchThread extends AsyncPrefetchThread {

        protected SparkPrefetchThread(@NonNull BlockingQueue<MultiDataSet> queue, @NonNull MultiDataSetIterator iterator, @NonNull MultiDataSet terminator, int deviceId) {
            super(queue, iterator, terminator, deviceId);
        }
    }
}
