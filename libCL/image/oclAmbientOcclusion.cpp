// Copyright [2011] [Geist Software Labs Inc.]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.#ifndef _oclAmbientOcclusion
#include "oclAmbientOcclusion.h"

oclAmbientOcclusion::oclAmbientOcclusion(oclContext& iContext)
: oclProgram(iContext, "oclAmbientOcclusion")
, clSSAO(*this)
{
	addSourceFile("image\\oclAmbientOcclusion.cl");

	exportKernel(clSSAO);
}

//
//
//

int oclAmbientOcclusion::compile()
{
	clSSAO = 0;

	if (!oclProgram::compile())
	{
		return 0;
	}

	clSSAO = createKernel("clSSAO");
	KERNEL_VALIDATE(clSSAO)
	return 1;
}

//
//
//

int oclAmbientOcclusion::compute(oclDevice& iDevice, oclImage2D& bfImage, oclImage2D& bfDepth)
{
	clSetKernelArg(clSSAO, 0, sizeof(cl_mem), bfImage);
	clSetKernelArg(clSSAO, 1, sizeof(cl_mem), bfDepth);
	clSetKernelArg(clSSAO, 2, sizeof(cl_mem), bfImage);

	size_t lGlobalSize[2];
	lGlobalSize[0] = bfImage.getImageInfo<size_t>(CL_IMAGE_WIDTH);
	lGlobalSize[1] = bfImage.getImageInfo<size_t>(CL_IMAGE_HEIGHT);
	sStatusCL = clEnqueueNDRangeKernel(iDevice, clSSAO, 2, NULL, lGlobalSize, NULL, 0, NULL, clSSAO.getEvent());
	ENQUEUE_VALIDATE

	return true;
}