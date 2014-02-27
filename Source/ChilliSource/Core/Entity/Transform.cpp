//
//  Transform.cpp
//  moFloTest
//
//  Created by Scott Downie on 24/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Entity/Transform.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------
		/// Constructor
        ///
        /// Default
		//----------------------------------------------------------------
        Transform::Transform() : mbIsTransformCacheValid(false), mbIsParentTransformCacheValid(false), mvScale(1,1,1), mpParentTransform(nullptr), mfOpacity(1.0f)
        {
        
        }
        //----------------------------------------------------------
		/// Set Look At
		///
		/// Set the camera orientation, target and position
		/// @param Position
		/// @param Look target
		/// @param Up direction
		//----------------------------------------------------------
		void Transform::SetLookAt(const Core::Vector3& invPos, const Core::Vector3& invTarget, const Core::Vector3& invUp)
		{
            Core::Vector3 vUp(invUp);
            
            Core::Vector3 vForward(invPos - invTarget);
            vForward.Normalise();
            
            Core::Vector3 vRight(vUp.CrossProduct(vForward));
            Core::Vector3::CrossProduct(&vForward, &vRight, &vUp);
            
            vUp.Normalise();
            vRight.Normalise();
            
            Core::Quaternion cRot(vRight, vUp, vForward);
            cRot.NormaliseSelf();
            
            SetPositionScaleOrientation(invPos, mvScale, cRot);
		}
        //----------------------------------------------------------------
        /// Set Position Scale Orientation
        ///
        /// @param Position vector
        /// @param Scale vector
        /// @param Orientation quaternion
        //----------------------------------------------------------------
        void Transform::SetPositionScaleOrientation(const Vector3& invPos, const Vector3& invScale, const Quaternion& invOrientation)
        {
            mvPosition = invPos;
            mvScale = invScale;
            mqOrientation = invOrientation;
            
            OnTransformChanged();
        }
        //----------------------------------------------------------------
		/// Set Position
		///
		/// @param X Component
		/// @param Y Component
		/// @param Z Component
		//----------------------------------------------------------------
		void Transform::SetPosition(f32 infX, f32 infY, f32 infZ)
		{
            if(mvPosition.x == infX && mvPosition.y == infY && mvPosition.z == infZ)
                return;
            
			mvPosition.x = infX;
			mvPosition.y = infY;
			mvPosition.z = infZ;
            
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Set Position
		///
		/// @param Position vector
		//----------------------------------------------------------------
		void Transform::SetPosition(const Vector3 &invPos)
		{
            if(mvPosition == invPos)
                return;
            
			mvPosition = invPos;
            
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Get Local Position
		///
		/// @return The position of the object 
		//----------------------------------------------------------------
		const Vector3& Transform::GetLocalPosition() const
		{
			return mvPosition;
		}
		//----------------------------------------------------------------
		/// Get Local Scale
		///
		/// @return The scale of object
		//----------------------------------------------------------------
		const Vector3& Transform::GetLocalScale() const
		{
            return mvScale;
		}
        //----------------------------------------------------------------
		/// Get World Position
		///
		/// @return The relative position of the object 
		//----------------------------------------------------------------
		const Vector3& Transform::GetWorldPosition() const
		{
            if(mpParentTransform)
            {
                mvWorldPosition = GetWorldTransform().GetTranslation();
                return mvWorldPosition;
            }
            
			return mvPosition;
		}
		//----------------------------------------------------------------
		/// Get World Scale
		///
		/// @return The relative scale of object
		//----------------------------------------------------------------
		const Vector3& Transform::GetWorldScale() const
		{
            if(mpParentTransform)
			{
                Vector3::Multiply(&mpParentTransform->GetWorldScale(), &mvScale, &mvWorldScale);
                return mvWorldScale;
			}
			
			return mvScale;
		}
        //----------------------------------------------------------------
		/// Get Local Orientation
		///
		/// @return Orientation quaternion
		//----------------------------------------------------------------
		const Quaternion& Transform::GetLocalOrientation() const
		{
			return mqOrientation;
		}
        //----------------------------------------------------------------
		/// Get World Orientation
		///
		/// @return Orientation quaternion relative to parent
		//----------------------------------------------------------------
		const Quaternion& Transform::GetWorldOrientation() const
		{
            if(mpParentTransform)
            {
                mqWorldOrientation = Core::Quaternion(GetWorldTransform());
                return mqWorldOrientation;
            }
            
            return mqOrientation;
		}
		//----------------------------------------------------------------
		/// Move By
		///
		/// @param X Component
		/// @param Y Component
		/// @param Z Component
		//----------------------------------------------------------------
		void Transform::MoveBy(f32 infX, f32 infY, f32 infZ)
		{
			mvPosition.x += infX;
			mvPosition.y += infY;
			mvPosition.z += infZ;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Move By
		///
		/// Movement direction vector
		//----------------------------------------------------------------
		void Transform::MoveBy(const Vector3 &invPos)
		{
			mvPosition += invPos;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Set Orientation
		///
		/// @param Orientation quaternion
		//----------------------------------------------------------------
		void Transform::SetOrientation(const Quaternion & inqOrientation)
		{
            if(mqOrientation == inqOrientation)
                return;
            
			mqOrientation = inqOrientation;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Rotate X By
		///
		/// Rotate around the x-axis
		///
		/// @param Angle to rotate in radians
		//----------------------------------------------------------------
		void Transform::RotateXBy(f32 infAngleRads)
		{			
			RotateBy(Vector3::X_UNIT_POSITIVE, infAngleRads);
		}
		//----------------------------------------------------------------
		/// Rotate Y By
		///
		/// Rotate around the y-axis
		///
		/// @param Angle to rotate in radians
		//----------------------------------------------------------------
		void Transform::RotateYBy(f32 infAngleRads)
		{
			RotateBy(Vector3::Y_UNIT_POSITIVE, infAngleRads);
		}
		//----------------------------------------------------------------
		/// Rotate Z By
		///
		/// Rotate around the z-axis
		///
		/// @param Angle to rotate in radians
		//----------------------------------------------------------------
		void Transform::RotateZBy(f32 infAngleRads)
		{
			RotateBy(Vector3::Z_UNIT_POSITIVE, infAngleRads);
		}
		//----------------------------------------------------------------
		/// Rotate By
		///
		/// Rotate around the arbitary axis
		///
		/// @param X-axis Component
		/// @param Y-axis Component
		/// @param Z-axis Component
		/// @param Angle in radians
		//----------------------------------------------------------------
		void Transform::RotateBy(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads)
		{			
			RotateBy(Vector3(inXAxis,inYAxis,inZAxis), infAngleRads);
		}
		//----------------------------------------------------------------
		/// Rotate By
		///
		/// Rotate around the arbitary axis
		///
		/// @param Axis vector
		/// @param Angle in radians
		//----------------------------------------------------------------
		void Transform::RotateBy(const Vector3 &vAxis, f32 infAngleRads)
		{
			mqOrientation = mqOrientation * Quaternion(vAxis,infAngleRads);
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Rotate To
		///
		/// Rotate about the given axis to the given angle 
		///
		/// @param X-axis Component
		/// @param Y-axis Component
		/// @param Z-axis Component
		/// @param Angle in radians
		//----------------------------------------------------------------
		void Transform::RotateTo(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads)
		{
			RotateTo(Vector3(inXAxis,inYAxis,inZAxis),infAngleRads);
		}
		//----------------------------------------------------------------
		/// Rotate To
		///
		/// Rotate about the given axis to the given angle 
		///
		/// @param Axis vector
		/// @param Angle in radians
		//----------------------------------------------------------------
		void Transform::RotateTo(const Vector3 &vAxis, f32 infAngleRads)
		{
			mqOrientation = Quaternion(vAxis,infAngleRads);
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Scale By
		///
		/// Scale uniformly by the given amount
		///
		/// @param Scale factor
		//----------------------------------------------------------------
		void Transform::ScaleBy(f32 inScale)
		{
			mvScale.x *= inScale;
			mvScale.y *= inScale;
			mvScale.z *= inScale;
						  
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Scale By
		///
		/// Scale along each axis by the given amount
		///
		/// @param X Component
		/// @param Y Component
		/// @param Z Component
		//----------------------------------------------------------------
		void Transform::ScaleBy(f32 inX, f32 inY, f32 inZ)
		{
			mvScale.x *= inX;
			mvScale.y *= inY;
			mvScale.z *= inZ;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Scale By
		///
		/// Scale along each axis by the given amount
		///
		/// @param Axis vector
		//----------------------------------------------------------------
		void Transform::ScaleBy(const Vector3 &Vec)
		{
			mvScale.x *= Vec.x;
			mvScale.y *= Vec.y;
			mvScale.z *= Vec.z;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Scale To
		///
		/// Scale uniformly to the given amount
		///
		/// @param Scale factor
		//----------------------------------------------------------------
		void Transform::ScaleTo(f32 inScale)
		{
            if(mvScale.x == inScale && mvScale.y == inScale && mvScale.z == inScale)
                return;
            
			mvScale.x = inScale;
			mvScale.y = inScale;
			mvScale.z = inScale;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Scale To
		///
		/// Scale each axis to the given amount
		///
		/// @param X Component
		/// @param Y Component
		/// @param Z Component
		//----------------------------------------------------------------
		void Transform::ScaleTo(f32 inX, f32 inY, f32 inZ)
		{
            if(mvScale.x == inX && mvScale.y == inY && mvScale.z == inZ)
                return;
            
			mvScale.x = inX;
			mvScale.y = inY;
			mvScale.z = inZ;
			
			OnTransformChanged();
		}
		//----------------------------------------------------------------
		/// Scale To
		///
		/// Scale each axis to the given amount
		///
		/// @param Axis vector
		//----------------------------------------------------------------
		void Transform::ScaleTo(const Vector3 &Vec)
		{
            if(mvScale == Vec)
                return;
            
			mvScale = Vec;
			
			OnTransformChanged();
		}
        
        //----------------------------------------------------------------
        /// Fade By
        ///
        /// Change of the opacity of the object
        ///
        /// @param inFade The amount to fade by
        //----------------------------------------------------------------
        void Transform::FadeBy(f32 inFade)
        {
            mfOpacity -= inFade;
            mfOpacity = std::min(std::max(mfOpacity, 0.0f), 1.0f);
            
            OnTransformChanged();

        }
        //----------------------------------------------------------------
        /// Scale To
        ///
        /// Fade uniformly to the given amount
        ///
        /// @param Fade factor
        //----------------------------------------------------------------
        void Transform::FadeTo(f32 inFade)
        {
            if(mfOpacity == inFade)
                return;
            
            mfOpacity = inFade;
            mfOpacity = std::min(std::max(mfOpacity, 0.0f), 1.0f);
            
            OnTransformChanged();
        }
        
        //----------------------------------------------------------------
        /// Get the opacity
        ///
        /// @return The opacity of object
        //----------------------------------------------------------------
        const f32 Transform::GetLocalOpacity() const
        {
            return mfOpacity;
        }
        
        //----------------------------------------------------------------
        /// Get World opacity
        ///
        /// @return The relative opacity of object
        //----------------------------------------------------------------
        const f32 Transform::GetWorldOpacity() const
        {
            f32 fParentOpacity = 1.0f;
            if(mpParentTransform)
                fParentOpacity = mpParentTransform->GetWorldOpacity();
            return mfOpacity*fParentOpacity;
        }
        
        //----------------------------------------------------------------
        /// Get Local Transform
        ///
        /// @return The currently cached transform (rebuilds if invalid)
        //----------------------------------------------------------------
        const Matrix4x4& Transform::GetLocalTransform() const
        {
            //Check if the transform needs to be re-calculated
            if(!mbIsTransformCacheValid)
            {
                mbIsTransformCacheValid = true;
                mmatTransform.SetTransform(mvPosition, mvScale, mqOrientation);
            }
            
            return mmatTransform;
        }
        //----------------------------------------------------------------
        /// Get World Transform
        ///
        /// @return The tranform in relation to its parent transform
        //----------------------------------------------------------------
        const Matrix4x4& Transform::GetWorldTransform() const
        {
            //If we have a parent transform we must apply it to
            //our local transform to get the relative transformation
            if(mpParentTransform)
            {
                //Our parent has changed so we must re-calculate our transform
                if(!mbIsParentTransformCacheValid)
                {
                    mbIsParentTransformCacheValid = true;
					
                    //Calculate the relative transform with our new parent transform
                    Matrix4x4::Multiply(&GetLocalTransform(), &mpParentTransform->GetWorldTransform(), &mmatWorldTransform);
                }
                //Our local transform has changed therefore we must update
                else if(!mbIsTransformCacheValid)
                {
                    //Calculate the relative transformation from our cached parent
                    Matrix4x4::Multiply(&GetLocalTransform(), &mpParentTransform->GetWorldTransform(), &mmatWorldTransform);
                }
            }
            //We do not have a parent so our relative transform is actually just our local one
            else if(!mbIsTransformCacheValid)
            {
                mmatWorldTransform = GetLocalTransform();
            }
            
			return mmatWorldTransform;
        }
        //----------------------------------------------------------------
        /// Set World Transform
        ///
        /// This will overwrite any parent or previous transformations
        ///
        /// @param Objects transformation matrix
        //----------------------------------------------------------------
        void Transform::SetWorldTransform(const Matrix4x4& inmatTransform)
        {
            inmatTransform.DecomposeTransforms(mvWorldPosition, mvWorldScale, mqWorldOrientation);
            
            mmatWorldTransform = inmatTransform;
            
            OnTransformChanged();
            
            mbIsTransformCacheValid = true;
            mbIsParentTransformCacheValid = true;
        }
        //----------------------------------------------------------------
        /// Set Local Transform
        ///
        /// This will overwrite any local previous transformations
        ///
        /// @param Objects transformation matrix
        //----------------------------------------------------------------
        void Transform::SetLocalTransform(const Matrix4x4& inmatTransform)
        {
            inmatTransform.DecomposeTransforms(mvPosition, mvScale, mqOrientation);
            
            mmatTransform = inmatTransform;
            
            OnTransformChanged();
            
            mbIsTransformCacheValid = false;
        }
        //----------------------------------------------------------------
        /// Is Transform Valid
        ///
        /// @return Whether the transform cache has been invalidated 
        //----------------------------------------------------------------
        bool Transform::IsTransformValid() const
        {
            return mbIsTransformCacheValid && mbIsParentTransformCacheValid;
        }
        //----------------------------------------------------------------
        /// Set Parent Transform
        ///
        /// Link this transform to another so that our transformations
        /// become relative to the parent transform
        ///
        /// @param Transform object
        //----------------------------------------------------------------
        void Transform::SetParentTransform(Transform* inpTransform)
        {
            mpParentTransform = inpTransform;
            
            OnParentTransformChanged();
        }
		//----------------------------------------------------------------
		/// Get Parent Transform
		/// @return what it says on tin
		//----------------------------------------------------------------
		Transform* Transform::GetParentTransform() const
		{
			return mpParentTransform;
		}
		//----------------------------------------------------------------
		/// Add Child Transform
		///
		/// Link this transform to another so that our transformations
		/// influence the childs transform
		///
		/// @param Transform object
		//----------------------------------------------------------------
		void Transform::AddChildTransform(Transform* inpTransform)
		{
			inpTransform->SetParentTransform(this);
			mChildTransforms.push_back(inpTransform);
		}
		//----------------------------------------------------------------
		/// Remove Child Transform
		///
		/// Unlink this transform to another so that our transformations
		/// no longer influence the childs transform
		///
		/// @param Transform object
		/// @return Whether the child was removed successfully
		//----------------------------------------------------------------
		bool Transform::RemoveChildTransform(Transform* inpTransform)
		{
			std::vector<Transform*>::iterator it = std::find(mChildTransforms.begin(), mChildTransforms.end(), inpTransform);
			
			if(it != mChildTransforms.end())
			{
				inpTransform->SetParentTransform(nullptr);
				mChildTransforms.erase(it);
				return true;
			}
			
			return false;
		}
		//----------------------------------------------------------------
		/// Remove All Child Transforms
		///
		/// Unlink this transform from all children so that our transformations
		/// no longer influence the childs transform
		//----------------------------------------------------------------
		void Transform::RemoveAllChildTransforms()
		{
			for(std::vector<Transform*>::iterator it = mChildTransforms.begin(); it != mChildTransforms.end(); ++it)
			{
				(*it)->SetParentTransform(nullptr);
			}
			
			mChildTransforms.clear();
		}
        //----------------------------------------------------------------
        /// Get Tranform Changed Event
        ///
        /// Subscribe to this event for notifications of when this
        /// transform is invalidated
        ///
        /// @return TransformChangedDelegate event
        //----------------------------------------------------------------
        IConnectableEvent<Transform::TransformChangedDelegate>& Transform::GetTransformChangedEvent()
        {
            return mTransformChangedEvent;
        }
        //----------------------------------------------------------------
        /// On Transform Changed 
        ///
        /// Triggered when our transform changes so we can 
        /// notify any dependant transforms
        //----------------------------------------------------------------
        void Transform::OnTransformChanged()
        {
            mbIsTransformCacheValid = false;
            
            for(std::vector<Transform*>::iterator it = mChildTransforms.begin(); it != mChildTransforms.end(); ++it)
            {
                (*it)->OnParentTransformChanged();
            }
            
            mTransformChangedEvent.NotifyConnections();
        }
        //----------------------------------------------------------------
        /// On Parent Transform Changed 
        ///
        /// Triggered when our parent transform changes so we can 
        /// recalculate our transform
        //----------------------------------------------------------------
        void Transform::OnParentTransformChanged()
        {
            mbIsParentTransformCacheValid = false;
            
            OnTransformChanged();
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Transform::Reset()
        {
            mbIsTransformCacheValid = false;
            mbIsParentTransformCacheValid = false;
            mvPosition = Vector3::ZERO;
            mvScale = Vector3::ONE;
            mqWorldOrientation = Quaternion::IDENTITY;
            mpParentTransform = nullptr;
            mfOpacity = 1.0f;
            mChildTransforms.clear();
            mTransformChangedEvent.CloseAllConnections();
        }
    }
}
