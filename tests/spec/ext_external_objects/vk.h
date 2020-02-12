/*
 * Copyright © 2020 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author:
 *    Eleni Maria Stea <estea@igalia.com>
 */

#ifndef VK_H
#define VK_H

#include <vulkan/vulkan.h>
#include <piglit-util.h>

struct vk_ctx
{
	VkInstance inst;
	VkPhysicalDevice pdev;
	VkDevice dev;
	VkPipelineCache cache;
	VkCommandPool cmd_pool;
	VkCommandBuffer cmd_buf;
	VkQueue queue;
};

struct vk_image_props
{
	uint32_t w;
	uint32_t h;
	uint32_t depth;

	uint32_t num_samples;
	uint32_t num_levels;
	uint32_t num_layers;

	VkFormat format;
	VkImageUsageFlagBits usage;
	VkImageTiling tiling;

	VkImageLayout in_layout;
	VkImageLayout end_layout;
};

struct vk_image_obj
{
	VkImage img;
	VkDeviceMemory mem;
	VkDeviceSize mem_sz;
};

struct vk_image_att {
	struct vk_image_obj obj;
	struct vk_image_props props;
};

struct vk_renderer
{
	VkPipeline pipeline;
	VkPipelineLayout pipeline_layout;
	VkRenderPass renderpass;
	VkShaderModule vs;
	VkShaderModule fs;
	VkFramebuffer fb;
};

struct vk_buf
{
	VkBuffer buf;
	VkDeviceMemory mem;
};

struct vk_semaphores
{
	VkSemaphore vk_frame_ready;
	VkSemaphore gl_frame_done;
};

struct vk_dims
{
	float w;
	float h;
};

bool
vk_init_ctx(struct vk_ctx *ctx);

bool
vk_init_ctx_for_rendering(struct vk_ctx *ctx);

void
vk_cleanup_ctx(struct vk_ctx *ctx);

bool
vk_create_ext_image(struct vk_ctx *ctx,
		    struct vk_image_props *props,
		    struct vk_image_obj *img_obj);

void vk_destroy_ext_image(struct vk_ctx *ctx, struct vk_image_obj *img_obj);

bool
vk_fill_ext_image_props(struct vk_ctx *ctx,
			uint32_t w, uint32_t h,
			uint32_t depth,
			uint32_t num_samples,
			uint32_t num_levels,
			uint32_t num_layers,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlagBits usage,
			VkImageLayout in_layout,
			VkImageLayout end_layout,
			struct vk_image_props *props);

bool
vk_create_renderer(struct vk_ctx *ctx,
		   const char *vs_src,
		   unsigned int vs_size,
		   const char *fs_src,
		   unsigned int fs_size,
		   struct vk_image_att *color_att,
		   struct vk_image_att *depth_att,
		   struct vk_renderer *renderer);

void
vk_destroy_renderer(struct vk_ctx *ctx,
		    struct vk_renderer *pipeline);

bool
vk_create_buffer(struct vk_ctx *ctx,
		 uint32_t sz,
		 VkBufferUsageFlagBits usage,
		 struct vk_buf *bo);

bool
vk_update_buffer_data(struct vk_ctx *ctx,
		      void *data,
		      uint32_t data_sz,
		      struct vk_buf *bo);

void
vk_destroy_buffer(struct vk_ctx *ctx,
		  struct vk_buf *bo);

void
vk_draw(struct vk_ctx *ctx,
	struct vk_buf *vbo,
	struct vk_renderer *renderer,
	float *vk_fb_color,
	uint32_t vk_fb_color_count,
	struct vk_semaphores *semaphores,
	bool has_wait, bool has_signal,
	uint32_t w, uint32_t h);

bool
vk_create_semaphores(struct vk_ctx *ctx,
		     struct vk_semaphores *semaphores);

void
vk_destroy_semaphores(struct vk_ctx *ctx,
		      struct vk_semaphores *semaphores);

#endif /* VK_H */
