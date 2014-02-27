/*
 * File: AudioManager.h
 * Date: 16/11/2010 2010 
 * Description: Handles the loading, caching and destroying of audio resources. It is a resource manager
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_AUDIO_MANAGER_H_
#define _MO_FLO_AUDIO_AUDIO_MANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Audio/Base/AudioResource.h>
#include <ChilliSource/Audio/3D/AudioListener.h>

namespace ChilliSource 
{
	namespace Audio 
	{
		class AudioManager : public Core::ResourceManager
		{
		public:
			DECLARE_NAMED_INTERFACE(AudioManager);
			
			AudioManager(){}
			virtual ~AudioManager(){}
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			virtual Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			virtual Core::InterfaceIDType GetProviderType() const override;
		    //----------------------------------------------------------------
		    /// Manages Resource Of Type
		    ///
		    /// @return Whether this object manages the object of type
		    //----------------------------------------------------------------
			virtual bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const override;
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//----------------------------------------------------------------
			/// Get Sound From File
			///
			/// Creates (lazily loads) a sound effect instance from file. If the 
			/// sound is already loaded it returns a handle to it
            ///
            /// @param The storage location to load from
            /// @param the file path
            /// @return the audio resource pointer
			//----------------------------------------------------------------
			AudioResourceSPtr GetSoundFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath);
			//----------------------------------------------------------------
			/// Stream Sound From File
			///
			/// Starts streaming audio from file
            ///
            /// @param The storage location to load from
            /// @param the file path
            /// @return the audio resource pointer
			//----------------------------------------------------------------
			AudioResourceSPtr StreamSoundFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath);
			//----------------------------------------------------------------
			/// Create Listener
			///
			/// @return Concrete audio listener
			//----------------------------------------------------------------
			AudioListenerSPtr CreateListener();
			//-----------------------------------------------------------------
			/// Destroy
			///
			/// @param Handle to the sound effect you want to destroy
			//-----------------------------------------------------------------
			void Destroy(const Core::ResourceSPtr& inpSoundEffect);
			//-----------------------------------------------------------------
			/// Destroy All 
			///
			//-----------------------------------------------------------------
			void DestroyAll();
			
		protected:
			
			//-----------------------------------------------------------------
			/// Create Audio Resource
			///
			/// @return Concrete audio resource based on the audio system
			//-----------------------------------------------------------------
			virtual AudioResourceSPtr CreateAudioResource() const = 0;
			
		private:

			typedef std::unordered_map<std::string, AudioResourceSPtr> MapStringToSoundEffectPtr;
			typedef MapStringToSoundEffectPtr::iterator MapStringToSoundEffectPtrItr;
			MapStringToSoundEffectPtr mMapFileNamesToSoundEffect;
		};
	}
}

#endif