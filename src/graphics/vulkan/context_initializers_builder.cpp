#include "context_initializers_builder.hpp"
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
        getAllDeviceMemories <<                                       //
        bindMemories;

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
                  getBufferMemories &&        //
                  bindBufferMemories);

        return {getInstance,        getPhysicalDevice,      getDevice,
                getSurface,         getSwapchain,           getRenderPass,
                getSwapchainImages, getSwapchainImageViews, getSwapchainFramebuffers,
                getShaderModules,   getDescriptorPool,      getDescriptorSetLayouts,
                getDescriptorSets,  getPipelineLayouts,     getGraphicsPipelines,
                getCommandPools,    getCommandBuffers,      getBuffers,
                getBufferMemories,  bindBufferMemories};
    }
} // namespace nd::src::graphics::vulkan
