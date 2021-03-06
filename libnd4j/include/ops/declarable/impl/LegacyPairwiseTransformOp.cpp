/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// Created by raver119 on 16.10.2017.
//

#include <helpers/ShapeUtils.h>
#include <ops/declarable/LegacyPairwiseTransformOp.h>


namespace nd4j {
    namespace ops {
        template <typename T>
        LegacyPairwiseTransformOp<T>::LegacyPairwiseTransformOp() : LegacyOp<T>::LegacyOp(2) {
            // just a no-op
        }

        template <typename T>
        LegacyPairwiseTransformOp<T>::LegacyPairwiseTransformOp(int opNum) : LegacyOp<T>::LegacyOp(2, opNum) {
            // just a no-op
        }

        template <typename T>
        LegacyOp<T>* LegacyPairwiseTransformOp<T>::clone() {
            return new LegacyPairwiseTransformOp(this->_opNum);
        }

        template <typename T>
        Nd4jStatus LegacyPairwiseTransformOp<T>::validateAndExecute(Context<T> &block) {
            auto x = INPUT_VARIABLE(0);
            auto y = INPUT_VARIABLE(1);
            auto z = OUTPUT_VARIABLE(0);

            if (!x->isSameShape(y)) {
                std::string sx = ShapeUtils<T>::shapeAsString(x);
                std::string sy = ShapeUtils<T>::shapeAsString(y);
                REQUIRE_TRUE(x->isSameShape(y) || y->isScalar(), 0, "Node_%i: For Pairwise transforms shapes of both operands should be equal but got %s vs %s", block.getNodeId(), sx.c_str(), sy.c_str());
            }

            int opNum = block.opNum() < 0 ? this->_opNum : block.opNum();

            NativeOpExcutioner<T>::execPairwiseTransform(opNum, x->getBuffer(), x->getShapeInfo(), y->getBuffer(), y->getShapeInfo(), z->getBuffer(), z->getShapeInfo(), block.getTArguments()->data());

            STORE_RESULT(*z);

            return ND4J_STATUS_OK;
        }

        /**
        *   Output shape of PWT operations always the same as input[0] shape, no exclusions.
        */
        template <typename T>
        ShapeList *LegacyPairwiseTransformOp<T>::calculateOutputShape(ShapeList *inputShape, nd4j::graph::Context<T> &block) {
            auto inShape = inputShape->at(0);

            Nd4jLong *newShape;
            COPY_SHAPE(inShape, newShape);

            return SHAPELIST(newShape);
        }

        template class ND4J_EXPORT LegacyPairwiseTransformOp<float>;
        template class ND4J_EXPORT LegacyPairwiseTransformOp<double>;
        template class ND4J_EXPORT LegacyPairwiseTransformOp<float16>;
    }
}