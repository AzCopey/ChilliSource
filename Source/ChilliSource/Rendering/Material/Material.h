//
//  Material.h
//  Chilli Source
//  Created by Scott Downie on 14/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_MATERIAL_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_MATERIAL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

#include <array>
#include <unordered_map>

namespace ChilliSource
{
    //----------------------------------------------------------
    /// Holds the render state of an object. Used to organise
    /// the rendering of objects and to alter their surface
    /// appearance via lighting, texture, shader, etc.
    ///
    /// @author S Downie
    //----------------------------------------------------------
    class Material final : public Resource
    {
    public:
        CS_DECLARE_NAMEDTYPE(Material);
        
        //----------------------------------------------------------
        /// Describes the different shading types that materials can
        /// use. These affect the shaders that the render system
        /// will use when rendering objects with this material.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        enum class ShadingType
        {
            k_unlit,
            k_blinn,
            k_custom
        };
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------------
        ///Sets the shading type that this material uses.
        ///
        /// @author Ian Copland
        ///
        /// @param in_shadingType - The shading type to use.
        //----------------------------------------------------------
        void SetShadingType(ShadingType in_shadingType) noexcept;
        //----------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The shading type that this material uses.
        //----------------------------------------------------------
        ShadingType GetShadingType() const noexcept;
        //----------------------------------------------------------
        /// Clear the textures from the slots
        ///
        /// @author S Downie
        //----------------------------------------------------------
        void RemoveAllTextures();
        //----------------------------------------------------------
        /// Add the texture to the end of the list. The index of the
        /// texture in the list corresponds to the texture handle in
        /// the shader.
        ///
        /// NOTE: Due to devices supporting different numbers of
        /// textures it is possible that textures at the end will be
        /// ignored by the renderer.
        ///
        /// @author S Downie
        ///
        /// @param Texture 
        //----------------------------------------------------------
        void AddTexture(const TextureCSPtr& in_texture);
        //----------------------------------------------------------
        /// Overwrite an exisiting added texture with the given
        /// texture at the given index. If a texture does not
        /// already exist at that slot then it asserts
        ///
        /// @author S Downie
        ///
        /// @param Texture
        /// @param Index
        //----------------------------------------------------------
        void SetTexture(const TextureCSPtr& in_texture, u32 in_texIndex = 0);
        //----------------------------------------------------------
        /// Get Texture at the given index. Will assert
        /// if index is out of bounds.
        ///
        /// @author S Downie
        ///
        /// @param Texture index
        ///
        /// @return Texture or null
        //----------------------------------------------------------
        const TextureCSPtr& GetTexture(u32 in_texIndex = 0) const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Number of textures set on the material
        //----------------------------------------------------------
        u32 GetNumTextures() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the object has transparency enabled 
        //----------------------------------------------------------
        bool IsTransparencyEnabled() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Whether transparency is enabled
        //----------------------------------------------------------
        void SetTransparencyEnabled(bool in_enable);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the object has colour write enabled
        //----------------------------------------------------------
        bool IsColourWriteEnabled() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Whether colour write is enabled
        //----------------------------------------------------------
        void SetColourWriteEnabled(bool in_enable);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the object has depth write enabled 
        //----------------------------------------------------------
        bool IsDepthWriteEnabled() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Whether depth write is enabled
        //----------------------------------------------------------
        void SetDepthWriteEnabled(bool in_enable);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the object has depth test enabled 
        //----------------------------------------------------------
        bool IsDepthTestEnabled() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Whether depth test is enabled
        //----------------------------------------------------------
        void SetDepthTestEnabled(bool in_enable);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the object has face culling enabled 
        //----------------------------------------------------------
        bool IsFaceCullingEnabled() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Whether face culling is enabled
        //----------------------------------------------------------
        void SetFaceCullingEnabled(bool in_enable);
        //----------------------------------------------------------
        /// Tells the render system how to blend pixels based on the
        /// source and destination mode
        ///
        /// @author S Downie
        ///
        /// @param Source mode
        /// @param Destination mode
        //----------------------------------------------------------
        void SetBlendModes(BlendMode in_source, BlendMode in_dest);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Source mode of blending functions
        //----------------------------------------------------------
        BlendMode GetSourceBlendMode() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Dest BlendMode of blending functions
        //----------------------------------------------------------
        BlendMode GetDestBlendMode() const;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param The face of the polygon that should be culled
        /// if culling is enabled
        //----------------------------------------------------------
        void SetCullFace(CullFace in_face);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return The face of the polygon that should be culled
        /// if face culling is enabled
        //----------------------------------------------------------
        CullFace GetCullFace() const;
        //----------------------------------------------------------
        /// The emissive colour is used to simulate the light emitted
        /// by the surface of an object (effectively its colour)
        ///
        /// @author S Downie
        ///
        /// @param Emissive colour used in ambient pass
        //----------------------------------------------------------
        void SetEmissive(const Colour& in_emissive);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Emissive colour used in ambient pass
        //----------------------------------------------------------
        const Colour& GetEmissive() const;
        //----------------------------------------------------------
        /// The ambient colour is used to simulate the light absorbed
        /// by the object from light reflections in the scene.
        ///
        /// @author S Downie
        ///
        /// @param Ambient colour used in ambient pass
        //----------------------------------------------------------
        void SetAmbient(const Colour& in_ambient);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Ambient colour used in ambient pass
        //----------------------------------------------------------
        const Colour& GetAmbient() const;
        //----------------------------------------------------------
        /// The diffuse colour is used to simulate the light absorbed
        /// by the object directly from the light source.
        ///
        /// @author S Downie
        ///
        /// @param Diffuse light colour used in subsequent light passes
        //----------------------------------------------------------
        void SetDiffuse(const Colour& in_diffuse);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Diffuse light colour used in subsequent light passes
        //----------------------------------------------------------
        const Colour& GetDiffuse() const;
        //----------------------------------------------------------
        /// The specular colour is used to simulate the light reflected
        /// back by the object creating a highlight.
        ///
        /// @author S Downie
        ///
        /// @param Specular light colour used in subsequent light passes
        //----------------------------------------------------------
        void SetSpecular(const Colour& in_specular);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Specular light colour used in subsequent light passes
        //----------------------------------------------------------
        const Colour& GetSpecular() const;
        //-----------------------------------------------------------
        /// Sets a custom shader for the given vertex format. The
        /// shading type must be set to custom or this will assert.
        ///
        /// @author Ian Copland
        ///
        /// @param vertexFormat - The vertex format that the custom
        /// shader will be applied to.
        /// @param shader - The custom shader.
        //-----------------------------------------------------------
        void SetCustomShader(const VertexFormat& vertexFormat, const ShaderCSPtr& shader) noexcept;
        //-----------------------------------------------------------
        /// Set the value of the variable with the given name to the
        /// given value
        ///
        /// @author S Downie
        ///
        /// @param Variable name
        /// @param Float value
        //-----------------------------------------------------------
        void SetShaderVar(const std::string& in_varName, f32 in_value);
        //-----------------------------------------------------------
        /// Set the value of the variable with the given name to the
        /// given value
        ///
        /// @author S Downie
        ///
        /// @param Variable name
        /// @param Vec2 value
        //-----------------------------------------------------------
        void SetShaderVar(const std::string& in_varName, const Vector2& in_value);
        //-----------------------------------------------------------
        /// Set the value of the variable with the given name to the
        /// given value
        ///
        /// @author S Downie
        ///
        /// @param Variable name
        /// @param Vec3 value
        //-----------------------------------------------------------
        void SetShaderVar(const std::string& in_varName, const Vector3& in_value);
        //-----------------------------------------------------------
        /// Set the value of the variable with the given name to the
        /// given value
        ///
        /// @author S Downie
        ///
        /// @param Variable name
        /// @param Vec4 value
        //-----------------------------------------------------------
        void SetShaderVar(const std::string& in_varName, const Vector4& in_value);
        //-----------------------------------------------------------
        /// Set the value of the variable with the given name to the
        /// given value
        ///
        /// @author S Downie
        ///
        /// @param Variable name
        /// @param Mat4 value
        //-----------------------------------------------------------
        void SetShaderVar(const std::string& in_varName, const Matrix4& in_value);
        //-----------------------------------------------------------
        /// Set the value of the variable with the given name to the
        /// given value
        ///
        /// @author S Downie
        ///
        /// @param Variable name
        /// @param Colour value
        //-----------------------------------------------------------
        void SetShaderVar(const std::string& in_varName, const Colour& in_value);
        //-----------------------------------------------------------
        /// Generates the RenderMaterialGroup that describes this
        /// material. This generated RenderMaterialGroup is cached
        /// and only re-generated when necessary.
        ///
        /// This is not thread safe and should only be called from
        /// the main thread.
        ///
        /// @author Ian Copland
        ///
        /// @return The RenderMaterialGroup that describes this
        /// material.
        //-----------------------------------------------------------
        const RenderMaterialGroup* GetRenderMaterialGroup() const noexcept;
        
        ~Material() noexcept;
        
    private:
        friend class ResourcePool;
        //----------------------------------------------------------
        /// Factory method to create an new instance of an empty
        /// material resource. Only called by the resource pool
        ///
        /// @author S Downie
        //----------------------------------------------------------
        static MaterialUPtr Create();
        //----------------------------------------------------------
        /// Private constructor to ensure that the factory method
        /// is used
        ///
        /// @author S Downie
        //----------------------------------------------------------
        Material() noexcept;
        //----------------------------------------------------------
        /// Generates an unlit render material group using the
        /// current material setting. If any of the settings are
        /// not allowed in an unlit render material group then this
        /// will assert.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        void CreateUnlitRenderMaterialGroup() const noexcept;
        //----------------------------------------------------------
        /// Generates an blinn render material group using the
        /// current material setting. If any of the settings are
        /// not allowed in an blinn render material group then this
        /// will assert.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        void CreateBlinnRenderMaterialGroup() const noexcept;
        //----------------------------------------------------------
        /// Generates a custom render material group using the
        /// current material setting. 
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        void CreateCustomRenderMaterialGroup() const noexcept;
        //----------------------------------------------------------
        /// Destroys the render material group if there is one.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        void DestroyRenderMaterialGroup() const noexcept;
        //----------------------------------------------------------
        /// Checks if the current textures are still valid, i.e. the containing
        /// render textures match the ones cached on creation.
        ///
        /// @author HMcLaughlin
        ///
        /// @return If the current texture list is invalid
        //----------------------------------------------------------
        bool VerifyTexturesAreValid() const noexcept;
        
        std::vector<TextureCSPtr> m_textures;
        
        ShadingType m_shadingType = ShadingType::k_custom;
        
        Colour m_emissive;
        Colour m_ambient;
        Colour m_diffuse;
        Colour m_specular;
        
        BlendMode m_srcBlendMode;
        BlendMode m_dstBlendMode;
        CullFace m_cullFace;
        

        bool m_isAlphaBlendingEnabled = false;
        bool m_isColWriteEnabled = true;
        bool m_isDepthWriteEnabled = true;
        bool m_isDepthTestEnabled = true;
        bool m_isFaceCullingEnabled = true;
        
        ShaderCSPtr m_customShader;
        VertexFormat m_customShaderVertexFormat = VertexFormat::k_sprite;
        
        std::unordered_map<std::string, f32> m_floatVars;
        std::unordered_map<std::string, Vector2> m_vec2Vars;
        std::unordered_map<std::string, Vector3> m_vec3Vars;
        std::unordered_map<std::string, Vector4> m_vec4Vars;
        std::unordered_map<std::string, Matrix4> m_mat4Vars;
        std::unordered_map<std::string, Colour> m_colourVars;
        
        RenderMaterialGroupManager* m_renderMaterialGroupManager = nullptr;
        
        //TODO: When the material system is improved, remove the need to make this mutable.
        mutable bool m_isCacheValid = false;
        mutable bool m_isVariableCacheValid = true;
        mutable const RenderMaterialGroup* m_renderMaterialGroup = nullptr;
        mutable std::vector<const RenderTexture*> m_cachedRenderTextures;
    };
}

#endif
