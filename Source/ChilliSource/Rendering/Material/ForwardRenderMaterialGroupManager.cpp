//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
#include <ChilliSource/Rendering/Material/ForwardRenderMaterialGroupManager.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Base/ForwardRenderPasses.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource
{
    namespace
    {
        /// Creates a new unlit render material with the given settings.
        ///
        /// @param shader
        ///     The shader that should be used.
        /// @param renderTexture
        ///     The render texture.
        /// @param isTransparencyEnabled
        ///     Whether or not transparency is enabled.
        /// @param isColourWriteEnabled
        ///     Whether or not colour write is enabled.
        /// @param isDepthWriteEnabled
        ///     Whether or not depth write is enabled.
        /// @param isDepthTestEnabled
        ///     Whether or not the depth test will be performed.
        /// @param isFaceCullingEnabled
        ///     Whether or not face culling will be performed.
        /// @param sourceBlendMode
        ///     The source blend mode. This only applies if transparency is enabled.
        /// @param destinationBlendMode
        ///     The destination blend mode. This only applies if transparency is enabled.
        /// @param cullFace
        ///     The face which should be called. This only applies if face culling is enabled.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param emissiveColour
        ///     The ambient colour.
        ///
        /// @return The new RenderMaterial.
        ///
        RenderMaterialUPtr CreateUnlit(const ShaderCSPtr& shader, const RenderTexture* renderTexture, bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled,
                                       bool isFaceCullingEnabled, BlendMode sourceBlendMode, BlendMode destinationBlendMode, CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour) noexcept
        {
            auto renderShader = shader->GetRenderShader();
            std::vector<const RenderTexture*> renderTextures { renderTexture };
            auto diffuseColour = Colour::k_black;
            auto specularColour = Colour::k_black;
            RenderShaderVariablesUPtr renderShaderVariables = nullptr;
            
            return RenderMaterialUPtr(new RenderMaterial(renderShader, renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode,
                                                         destinationBlendMode, cullFace, emissiveColour, ambientColour, diffuseColour, specularColour, std::move(renderShaderVariables)));
        }
        
        /// Creates a new shadow map RenderMaterial with the given shader.
        ///
        /// @param shader
        ///     The shader that should be used.
        ///
        /// @return The new RenderMaterial.
        ///
        RenderMaterialUPtr CreateShadowMap(const ShaderCSPtr& shader) noexcept
        {
            auto renderShader = shader->GetRenderShader();
            std::vector<const RenderTexture*> renderTextures;
            auto isTransparencyEnabled = false;
            auto isColourWriteEnabled = false;
            auto isDepthWriteEnabled = true;
            auto isDepthTestEnabled = true;
            auto isFaceCullingEnabled = true;
            auto sourceBlendMode = BlendMode::k_one;
            auto destinationBlendMode = BlendMode::k_one;
            auto cullFace = CullFace::k_front;
            auto emissiveColour = Colour::k_black;
            auto ambientColour = Colour::k_black;
            auto diffuseColour = Colour::k_black;
            auto specularColour = Colour::k_black;
            RenderShaderVariablesUPtr renderShaderVariables = nullptr;
            
            return RenderMaterialUPtr(new RenderMaterial(renderShader, renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode,
                                                         destinationBlendMode, cullFace, emissiveColour, ambientColour, diffuseColour, specularColour, std::move(renderShaderVariables)));
        }
        
        /// Creates a new base pass blinn RenderMaterial with the given info.
        ///
        /// @param renderTexture
        ///     The render texture.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param emissiveColour
        ///     The ambient colour.
        /// @param diffuseColour
        ///     The diffuse colour.
        /// @param specularColour
        ///     The specular colour.
        ///
        /// @return The new RenderMaterial.
        ///
        RenderMaterialUPtr CreateBlinnBase(const ShaderCSPtr& shader, const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour) noexcept
        {
            auto renderShader = shader->GetRenderShader();
            std::vector<const RenderTexture*> renderTextures { renderTexture };
            auto isTransparencyEnabled = false;
            auto isColourWriteEnabled = true;
            auto isDepthWriteEnabled = true;
            auto isDepthTestEnabled = true;
            auto isFaceCullingEnabled = true;
            auto sourceBlendMode = BlendMode::k_one;
            auto destinationBlendMode = BlendMode::k_oneMinusSourceAlpha;
            auto cullFace = CullFace::k_back;
            auto diffuseColour = Colour::k_black;
            auto specularColour = Colour::k_black;
            RenderShaderVariablesUPtr renderShaderVariables = nullptr;
            
            return RenderMaterialUPtr(new RenderMaterial(renderShader, renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode,
                                                         destinationBlendMode, cullFace, emissiveColour, ambientColour, diffuseColour, specularColour, std::move(renderShaderVariables)));
        }
        
        /// Creates a new light pass (directional or point) blinn RenderMaterial with the given info.
        ///
        /// @param renderTexture
        ///     The render texture.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param emissiveColour
        ///     The ambient colour.
        /// @param diffuseColour
        ///     The diffuse colour.
        /// @param specularColour
        ///     The specular colour.
        ///
        /// @return The new RenderMaterial.
        ///
        RenderMaterialUPtr CreateBlinnLight(const ShaderCSPtr& shader, const RenderTexture* renderTexture, const Colour& diffuseColour, const Colour& specularColour) noexcept
        {
            auto renderShader = shader->GetRenderShader();
            std::vector<const RenderTexture*> renderTextures { renderTexture };
            auto isTransparencyEnabled = true;
            auto isColourWriteEnabled = true;
            auto isDepthWriteEnabled = false;
            auto isDepthTestEnabled = true;
            auto isFaceCullingEnabled = true;
            auto sourceBlendMode = BlendMode::k_one;
            auto destinationBlendMode = BlendMode::k_one;
            auto cullFace = CullFace::k_back;
            auto emissiveColour = Colour::k_black;
            auto ambientColour = Colour::k_black;
            RenderShaderVariablesUPtr renderShaderVariables = nullptr;
            
            return RenderMaterialUPtr(new RenderMaterial(renderShader, renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode,
                                                         destinationBlendMode, cullFace, emissiveColour, ambientColour, diffuseColour, specularColour, std::move(renderShaderVariables)));
        }
    }
    
    CS_DEFINE_NAMEDTYPE(ForwardRenderMaterialGroupManager);
    
    //------------------------------------------------------------------------------
    bool ForwardRenderMaterialGroupManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderMaterialGroupManager::InterfaceID == interfaceId || ForwardRenderMaterialGroupManager::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterialGroup* ForwardRenderMaterialGroupManager::CreateUnlitRenderMaterialGroup(const RenderTexture* renderTexture, bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled,
                                                                                                 bool isDepthTestEnabled, bool isFaceCullingEnabled, BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                                                                 CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour) noexcept
    {
        //TODO: Create RenderMaterials from pools.
        
        std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots> spriteRenderMaterials {};
        std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots> staticRenderMaterials {};
        std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots> animatedRenderMaterials {};
        std::vector<RenderMaterialUPtr> renderMaterials;
        
        if (isTransparencyEnabled)
        {
            auto spriteRM = CreateUnlit(m_spriteUnlit, renderTexture, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode, destinationBlendMode, cullFace,
                                        emissiveColour, ambientColour);
            spriteRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_transparent)] = spriteRM.get();
            renderMaterials.push_back(std::move(spriteRM));
            
            auto staticRM = CreateUnlit(m_staticUnlit, renderTexture, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode, destinationBlendMode,
                                        cullFace, emissiveColour, ambientColour);
            staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_transparent)] = staticRM.get();
            renderMaterials.push_back(std::move(staticRM));
            
            auto animatedRM = CreateUnlit(m_animatedUnlit, renderTexture, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode, destinationBlendMode,
                                        cullFace, emissiveColour, ambientColour);
            animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_transparent)] = animatedRM.get();
            renderMaterials.push_back(std::move(animatedRM));
        }
        else
        {
            //TODO: Create RenderMaterials from pools.
            auto spriteRM = CreateUnlit(m_spriteUnlit, renderTexture, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode, destinationBlendMode, cullFace,
                                        emissiveColour, ambientColour);
            spriteRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = spriteRM.get();
            renderMaterials.push_back(std::move(spriteRM));
            
            auto staticRM = CreateUnlit(m_staticUnlit, renderTexture, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode, destinationBlendMode,
                                        cullFace, emissiveColour, ambientColour);
            staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = staticRM.get();
            renderMaterials.push_back(std::move(staticRM));
            
            auto animatedRM = CreateUnlit(m_animatedUnlit, renderTexture, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode, destinationBlendMode,
                                          cullFace, emissiveColour, ambientColour);
            animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = animatedRM.get();
            renderMaterials.push_back(std::move(animatedRM));
            
            if (m_shadowsSupported)
            {
                auto staticShadowMapRM = CreateShadowMap(m_staticShadowMap);
                staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_shadowMap)] = staticShadowMapRM.get();
                renderMaterials.push_back(std::move(staticShadowMapRM));
                
                auto animatedShadowMapRM = CreateShadowMap(m_animatedShadowMap);
                animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_shadowMap)] = animatedShadowMapRM.get();
                renderMaterials.push_back(std::move(animatedShadowMapRM));
            }
        }
        
        std::vector<RenderMaterialGroup::Collection> collections
        {
            RenderMaterialGroup::Collection(VertexFormat::k_sprite, spriteRenderMaterials),
            RenderMaterialGroup::Collection(VertexFormat::k_staticMesh, staticRenderMaterials),
            RenderMaterialGroup::Collection(VertexFormat::k_animatedMesh, animatedRenderMaterials)
        };
        
        RenderMaterialGroupUPtr renderMaterialGroup(new RenderMaterialGroup(std::move(renderMaterials), collections));
        auto renderMaterialGroupRaw = renderMaterialGroup.get();
        AddRenderMaterialGroup(std::move(renderMaterialGroup));
        
        return renderMaterialGroupRaw;
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterialGroup* ForwardRenderMaterialGroupManager::CreateBlinnRenderMaterialGroup(const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour,
                                                                                                 const Colour& specularColour) noexcept
    {
        //TODO: Create RenderMaterials from pools.
        
        std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots> staticRenderMaterials {};
        std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots> animatedRenderMaterials {};
        std::vector<RenderMaterialUPtr> renderMaterials;
        
        auto staticBaseRM = CreateBlinnBase(m_staticBlinnBase, renderTexture, emissiveColour, ambientColour);
        staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = staticBaseRM.get();
        renderMaterials.push_back(std::move(staticBaseRM));
        
        auto staticDirectionalRM = CreateBlinnLight(m_staticBlinnDirectional, renderTexture, diffuseColour, specularColour);
        staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_directionalLight)] = staticDirectionalRM.get();
        renderMaterials.push_back(std::move(staticDirectionalRM));
        
        auto staticPointRM = CreateBlinnLight(m_staticBlinnPoint, renderTexture, diffuseColour, specularColour);
        staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_pointLight)] = staticPointRM.get();
        renderMaterials.push_back(std::move(staticPointRM));
        
        auto animatedBaseRM = CreateBlinnBase(m_animatedBlinnBase, renderTexture, emissiveColour, ambientColour);
        animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_base)] = animatedBaseRM.get();
        renderMaterials.push_back(std::move(animatedBaseRM));
        
        auto animatedDirectionalRM = CreateBlinnLight(m_animatedBlinnDirectional, renderTexture, diffuseColour, specularColour);
        animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_directionalLight)] = animatedDirectionalRM.get();
        renderMaterials.push_back(std::move(animatedDirectionalRM));
        
        auto animatedPointRM = CreateBlinnLight(m_animatedBlinnPoint, renderTexture, diffuseColour, specularColour);
        animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_pointLight)] = animatedPointRM.get();
        renderMaterials.push_back(std::move(animatedPointRM));
        
        if (m_shadowsSupported)
        {
            auto staticShadowMapRM = CreateShadowMap(m_staticShadowMap);
            staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_shadowMap)] = staticShadowMapRM.get();
            renderMaterials.push_back(std::move(staticShadowMapRM));
            
            auto staticDirectionalShadowsRM = CreateBlinnLight(m_staticBlinnDirectionalShadows, renderTexture, diffuseColour, specularColour);
            staticRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_directionalLightShadows)] = staticDirectionalShadowsRM.get();
            renderMaterials.push_back(std::move(staticDirectionalShadowsRM));
            
            auto animatedShadowMapRM = CreateShadowMap(m_animatedShadowMap);
            animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_shadowMap)] = animatedShadowMapRM.get();
            renderMaterials.push_back(std::move(animatedShadowMapRM));
            
            auto animatedDirectionalShadowsRM = CreateBlinnLight(m_animatedBlinnDirectionalShadows, renderTexture, diffuseColour, specularColour);
            animatedRenderMaterials[static_cast<u32>(ForwardRenderPasses::k_directionalLightShadows)] = animatedDirectionalShadowsRM.get();
            renderMaterials.push_back(std::move(animatedDirectionalShadowsRM));
        }
        
        std::vector<RenderMaterialGroup::Collection> collections
        {
            RenderMaterialGroup::Collection(VertexFormat::k_staticMesh, staticRenderMaterials),
            RenderMaterialGroup::Collection(VertexFormat::k_animatedMesh, animatedRenderMaterials)
        };

        RenderMaterialGroupUPtr renderMaterialGroup(new RenderMaterialGroup(std::move(renderMaterials), collections));
        auto renderMaterialGroupRaw = renderMaterialGroup.get();
        AddRenderMaterialGroup(std::move(renderMaterialGroup));
        
        return renderMaterialGroupRaw;
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterialGroup* ForwardRenderMaterialGroupManager::CreateCustomRenderMaterialGroup(const VertexFormat& vertexFormat, const RenderShader* renderShader, const std::vector<const RenderTexture*>& renderTextures,
                                                                                                  bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled,
                                                                                                  bool isFaceCullingEnabled, BlendMode sourceBlendMode, BlendMode destinationBlendMode, CullFace cullFace,
                                                                                                  const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour, const Colour& specularColour,
                                                                                                  RenderShaderVariablesUPtr renderShaderVariables) noexcept
    {
        //TODO: Create RenderMaterials from pools.
        
        std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots> renderMaterialsSlots {};
        std::vector<RenderMaterialUPtr> renderMaterials;
        
        if (isTransparencyEnabled)
        {
            auto renderMaterial = RenderMaterialUPtr(new RenderMaterial(renderShader, renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode,
                                                                        destinationBlendMode, cullFace, emissiveColour, ambientColour, diffuseColour, specularColour, std::move(renderShaderVariables)));
            renderMaterialsSlots[static_cast<u32>(ForwardRenderPasses::k_transparent)] = renderMaterial.get();
            renderMaterials.push_back(std::move(renderMaterial));
        }
        else
        {
            auto renderMaterial = RenderMaterialUPtr(new RenderMaterial(renderShader, renderTextures, isTransparencyEnabled, isColourWriteEnabled, isDepthWriteEnabled, isDepthTestEnabled, isFaceCullingEnabled, sourceBlendMode,
                                                                        destinationBlendMode, cullFace, emissiveColour, ambientColour, diffuseColour, specularColour, std::move(renderShaderVariables)));
            renderMaterialsSlots[static_cast<u32>(ForwardRenderPasses::k_base)] = renderMaterial.get();
            renderMaterials.push_back(std::move(renderMaterial));
            
            if (m_shadowsSupported)
            {
                if (m_shadowsSupported && vertexFormat == VertexFormat::k_staticMesh)
                {
                    auto shadowMapMaterial = CreateShadowMap(m_staticShadowMap);
                    renderMaterialsSlots[static_cast<u32>(ForwardRenderPasses::k_shadowMap)] = shadowMapMaterial.get();
                    renderMaterials.push_back(std::move(shadowMapMaterial));
                }
                else if(m_shadowsSupported && vertexFormat == VertexFormat::k_animatedMesh)
                {
                    auto shadowMapMaterial = CreateShadowMap(m_animatedShadowMap);
                    renderMaterialsSlots[static_cast<u32>(ForwardRenderPasses::k_shadowMap)] = shadowMapMaterial.get();
                    renderMaterials.push_back(std::move(shadowMapMaterial));
                }
            }
        }

        std::vector<RenderMaterialGroup::Collection> collections { RenderMaterialGroup::Collection(vertexFormat, renderMaterialsSlots) };
        
        RenderMaterialGroupUPtr renderMaterialGroup(new RenderMaterialGroup(std::move(renderMaterials), collections));
        auto renderMaterialGroupRaw = renderMaterialGroup.get();
        AddRenderMaterialGroup(std::move(renderMaterialGroup));
        
        return renderMaterialGroupRaw;
    }
    
    //------------------------------------------------------------------------------
    void ForwardRenderMaterialGroupManager::OnInit() noexcept
    {
        auto renderCapabilities = Application::Get()->GetSystem<RenderCapabilities>();
        m_shadowsSupported = renderCapabilities->IsShadowMappingSupported();
        
        auto resourcePool = Application::Get()->GetResourcePool();
        
        m_spriteUnlit = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Sprite-Unlit.csshader");
        
        m_staticUnlit = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Static-Unlit.csshader");
        m_staticBlinnBase = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Static-Blinn-Base.csshader");
        m_staticBlinnDirectional = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Static-Blinn-Directional.csshader");
        m_staticBlinnPoint = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Static-Blinn-Point.csshader");
        
        m_animatedUnlit = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Animated-Unlit.csshader");
        m_animatedBlinnBase = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Animated-Blinn-Base.csshader");
        m_animatedBlinnDirectional = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Animated-Blinn-Directional.csshader");
        m_animatedBlinnPoint = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Animated-Blinn-Point.csshader");
        
        if (m_shadowsSupported)
        {
            m_staticShadowMap = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Static-ShadowMap.csshader");
            m_staticBlinnDirectionalShadows = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Static-Blinn-DirectionalShadows.csshader");
            
            m_animatedShadowMap = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Animated-ShadowMap.csshader");
            m_animatedBlinnDirectionalShadows = resourcePool->LoadResource<Shader>(StorageLocation::k_chilliSource, "Shaders/Animated-Blinn-DirectionalShadows.csshader");
        }
    }
    
    //------------------------------------------------------------------------------
    void ForwardRenderMaterialGroupManager::OnDestroy() noexcept
    {
        m_spriteUnlit.reset();
        
        m_staticShadowMap.reset();
        m_staticUnlit.reset();
        m_staticBlinnBase.reset();
        m_staticBlinnDirectional.reset();
        m_staticBlinnPoint.reset();
        m_staticBlinnDirectionalShadows.reset();

        m_animatedShadowMap.reset();
        m_animatedUnlit.reset();
        m_animatedBlinnBase.reset();
        m_animatedBlinnDirectional.reset();
        m_animatedBlinnPoint.reset();
        m_animatedBlinnDirectionalShadows.reset();
    }
}
