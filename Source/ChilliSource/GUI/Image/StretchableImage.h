//
//  StretchableImage.h
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_STRETCHABLEIMAGEVIEW_H_
#define _MOFLO_GUI_STRETCHABLEIMAGEVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/GUI/Base/GUIView.h>

//=============================================================
/// GUI - Stretchable Image 
///
/// Creates a scalable image using the nine patch system
//=============================================================

namespace ChilliSource
{
    namespace GUI
    {
        class StretchableImage : public GUIView
        {
        public:

			DECLARE_META_CLASS(StretchableImage)

            struct PanelDesc
            {
                Core::Rectangle m_topLeftUVs;
                Core::Rectangle m_topCentreUVs;
                Core::Rectangle m_topRightUVs;
                
                Core::Rectangle m_bottomLeftUVs;
                Core::Rectangle m_bottomCentreUVs;
                Core::Rectangle m_bottomRightUVs;
                
                Core::Rectangle m_leftCentreUVs;
                Core::Rectangle m_middleCentreUVs;
                Core::Rectangle m_rightCentreUVs;
                
                Core::Vector2 m_topLeftSize;
                Core::Vector2 m_topCentreSize;
                Core::Vector2 m_topRightSize;
                
                Core::Vector2 m_bottomLeftSize;
                Core::Vector2 m_bottomCentreSize;
                Core::Vector2 m_bottomRightSize;
                
                Core::Vector2 m_leftCentreSize;
                Core::Vector2 m_middleCentreSize;
                Core::Vector2 m_rightCentreSize;
            };
            
            StretchableImage();
            StretchableImage(const Core::ParamDictionary& insParams);
            //---------------------------------------------------------
            /// Set Texture
            ///
            /// @param Texture containing the nine patches
            //---------------------------------------------------------
            void SetTexture(const Rendering::TextureCSPtr& inpTexture);
            //---------------------------------------------------------
            /// Get Texture
            ///
            /// @return Texture containing the nine patches
            //---------------------------------------------------------
            const Rendering::TextureCSPtr& GetTexture() const;
            //---------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Sprite sheet containing the nine patches
            //---------------------------------------------------------
            void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
            //---------------------------------------------------------
            /// Get Sprite Sheet
            ///
            /// @return Sprite sheet containing the nine patches
            //---------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
			//---------------------------------------------------------
			/// Set Base Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_TOP_LEFT"
			///			* "BLUE_PANEL_TOP_RIGHT"
			///			* "BLUE_PANEL_BOTTOM_LEFT"
			///			* "BLUE_PANEL_BOTTOM_RIGHT"
			///			* "BLUE_PANEL_TOP_CENTRE"
			///			* "BLUE_PANEL_BOTTOM_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_RIGHT"
			///			* "BLUE_PANEL_MIDDLE_LEFT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseTextureAtlasID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs.
			/// e.g. For the following IDs:
			///			* "BLUE_PANEL_TOP_LEFT"
			///			* "BLUE_PANEL_TOP_RIGHT"
			///			* "BLUE_PANEL_BOTTOM_LEFT"
			///			* "BLUE_PANEL_BOTTOM_RIGHT"
			///			* "BLUE_PANEL_TOP_CENTRE"
			///			* "BLUE_PANEL_BOTTOM_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_RIGHT"
			///			* "BLUE_PANEL_MIDDLE_LEFT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			const std::string& GetBaseTextureAtlasID() const;
            //---------------------------------------------------------
            /// Draw
            /// 
            /// Draw the image constructed from the nine patches
            ///
            /// @param Canvas renderer pointer
            //---------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* inpCanvas);
			//--------------------------------------------------------
            /// Set Width Maintaining Aspect
            ///
            /// Change the current width of the image view and resize the height
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified width
            //--------------------------------------------------------
            void SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth);
            //--------------------------------------------------------
            /// Set Height Maintaining Aspect
            ///
            /// Change the current height of the image view and resize the width
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified height
            //--------------------------------------------------------
            void SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Enable Width Maintaining Aspect
            ///
            /// Enables auto scaling of the Width to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableWidthMaintainingAspect(bool inbEnabled);
            //--------------------------------------------------------
            /// Enable Height Maintaining Aspect
            ///
            /// Enables auto scaling of the height to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableHeightMaintainingAspect(bool inbEnabled);
			//--------------------------------------------------------
            /// Is Width Maintaining Aspect Enabled
            ///
            /// @return auto scaling of the Width to maintain the aspect ratio
            //--------------------------------------------------------
            bool IsWidthMaintainingAspectEnabled() const;
            //--------------------------------------------------------
            /// Is Height Maintaining Aspect Enabled
            ///
            /// @return auto scaling of the height to maintain the aspect ratio
            //--------------------------------------------------------
            bool IsHeightMaintainingAspectEnabled() const;
			
            //--------------------------------------------------------
            /// Enable Centre Touch Consumption
            ///
            /// Enables the touch to go through the middle part of the image
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableCentreTouchConsumption(bool inbEnabled);
			//--------------------------------------------------------
            /// Is Centre Touch Consumption Enabled
            ///
            /// @return whether the touch though the middle is enabled or not
            //--------------------------------------------------------
            bool IsCentreTouchConsumptionEnabled() const;
			
			//---Touch Delegates
			//-----------------------------------------------------------
			/// Called when the window receives cursor/touch input
			///
            /// @author S Downie
            ///
			/// @param The pointer.
            /// @param The timestamp
            /// @param The press type.
            ///
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType) override;
			//-----------------------------------------------------------
			/// Called when the window receives cursor/touch input
			///
            /// @author S Downie
			///
			/// @param The pointer.
            /// @param The timestamp
            ///
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp) override;
            
            struct PatchSize
            {
				CSCore::Vector2 vSizeTopLeft;
				CSCore::Vector2 vSizeTopRight;
				CSCore::Vector2 vSizeBottomLeft;
				CSCore::Vector2 vSizeBottomRight;
				CSCore::Vector2 vSizeTopCentre;
				CSCore::Vector2 vSizeBottomCentre;
				CSCore::Vector2 vSizeLeftCentre;
				CSCore::Vector2 vSizeRightCentre;
				CSCore::Vector2 vSizeMiddleCentre;
            };
			
			void CalculatePatchSize(PatchSize& outSizePatch);
			
		private:

            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);

			DECLARE_PROPERTY_A(std::string, BaseTextureAtlasID, SetBaseTextureAtlasID, GetBaseTextureAtlasID);

			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, CentreTouchConsumption, EnableCentreTouchConsumption, IsCentreTouchConsumptionEnabled);
			
            PanelDesc m_panels;
        };
    }
}

#endif