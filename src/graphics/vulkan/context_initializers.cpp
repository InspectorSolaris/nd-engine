#include "context_initializers.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    auto initializersBuilder = VulkanContextInitializersBuilder {} << //
        getInstance <<                                                //
        getPhysicalDevice <<                                          //
        getDevice <<                                                  //
        getSwapchain <<                                               //
        getRenderPass <<                                              //
        getSwapchainImages <<                                         //
        getImageViews <<                                              //
        getFramebuffers <<                                            //
        getShaderModules <<                                           //
        getDescriptorPool <<                                          //
        getDescriptorSetLayouts <<                                    //
        getDescriptorSets <<                                          //
        getPipelineLayouts <<                                         //
        getGraphicsPipelines <<                                       //
        getCommandPools <<                                            //
        getCommandBuffers <<                                          //
        getBuffers <<                                                 //
        getDeviceMemories;

    VulkanContextInitializersBuilder::Type
    VulkanContextInitializersBuilder::build() const
    {
        ND_SET_SCOPE();

        ND_ASSERT(getInstance &&              //
                  getPhysicalDevice &&        //
                  getDevice &&                //
                  getSurface &&               //
                  getSwapchain &&             //
                  getRenderPass &&            //
                  getSwapchainImages &&       //
                  getSwapchainImageViews &&   //
                  getSwapchainFramebuffers && //
                  getShaderModules &&         //
                  getDescriptorPool &&        //
                  getDescriptorSetLayouts &&  //
                  getDescriptorSets &&        //
                  getPipelineLayouts &&       //
                  getGraphicsPipelines &&     //
                  getCommandPools &&          //
                  getCommandBuffers &&        //
                  getBuffers &&               //
                  getBufferMemories);

        return {getInstance,
                getPhysicalDevice,
                getDevice,
                getSurface,
                getSwapchain,
                getRenderPass,
                getSwapchainImages,
                getSwapchainImageViews,
                getSwapchainFramebuffers,
                getShaderModules,
                getDescriptorPool,
                getDescriptorSetLayouts,
                getDescriptorSets,
                getPipelineLayouts,
                getGraphicsPipelines,
                getCommandPools,
                getCommandBuffers,
                getBuffers,
                getBufferMemories};
    }
} // namespace nd::src::graphics::vulkan
