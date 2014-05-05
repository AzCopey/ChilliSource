//
//  ParticleEmitter.h
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_PARTICLE_EMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
    namespace Rendering
    {
        struct Particle
        {
            Core::Vector3 m_scale;
            Core::Vector3 m_translation;
			Core::Colour m_colour;				//The colour of the particle
            Core::Vector3 m_velocity;		//The speed of the particle in 3D space
            f32 m_energy;					//Measures the full lifetime of the particle from birth at 1 to death at 0
            f32 m_angularRotation;
        };
        
        class ParticleEmitter
        {
        public:
            
            ParticleEmitter(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent);
            virtual ~ParticleEmitter();
            //-----------------------------------------------------
            /// Update
            ///
            /// Update all the active particles with respect to
            /// time
            ///
            /// @param Time between frames
            //-----------------------------------------------------
            void Update(f32 infDT);
            //-----------------------------------------------------
            /// Render
            ///
            /// Render the active particles based on their
            /// renderable type
            ///
            /// @param Render system
            //-----------------------------------------------------
            void Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam);
            //---------------------------------------------------
            /// Start Emitting
            ///
            /// Particle emitters will resume emitting
            //---------------------------------------------------
            void StartEmitting();
            //---------------------------------------------------
            /// Emit Burst
            ///
            /// Particle emitters will fire particles once
            //---------------------------------------------------
            void EmitBurst();
            //---------------------------------------------------
            /// Stop Emitting
            ///
            /// No new particles will be emitted once the current
            /// ones die
            //---------------------------------------------------
            void StopEmitting();
            //-----------------------------------------------------
            /// Add Affector
            ///
            /// Add a new affector to apply to the particles
            ///
            /// @param Particle affector
            //-----------------------------------------------------
            void AddAffector(ParticleAffector* inpAffector);
            //-----------------------------------------------------
            /// Remove Affector
            ///
            /// Remove the affector so it no longer applies to
            /// the system
            ///
            /// @param Particle affector
            //-----------------------------------------------------
            void RemoveAffector(ParticleAffector* inpAffector);
            //-----------------------------------------------------
            /// Set Velocity
            ///
            /// Set the initial velocity of the particles leaving
            /// the emitter
            ///
            /// @param Velocity
            //-----------------------------------------------------
            void SetVelocity(f32 infVelocity);
            //-----------------------------------------------------
            /// Set Colour
            ///
            /// Set the initial colour of the particles
            ///
            /// @param Colour
            //-----------------------------------------------------
            void SetColour(const Core::Colour inColour);
            //-----------------------------------------------------
            /// Set Number of Particles Per Emission
            ///
            /// Sets the number of particles that are emitted
            /// at any given time. This is a maximum if there are
            /// no particles available no new ones will be created
            ///
            /// @param Number of particles
            //-----------------------------------------------------
            void SetNumParticlesPerEmission(u32 inudwNumParticles);
            //-----------------------------------------------------
            /// Set Material
            ///
            /// The material that each particle will use
            ///
            /// @param Material ptr
            //-----------------------------------------------------
            void SetMaterial(const MaterialCSPtr& inpMaterial);
            //-----------------------------------------------------
            /// Set Emission Frequency
            ///
            /// After what period in seconds should particles be
            /// emitted
            ///
            /// @param Frequency in seconds
            //-----------------------------------------------------
            void SetEmissionFrequency(f32 infFreq);
            //-----------------------------------------------------
            /// Set Lifetime
            ///
            /// Set the lifetime of the particles in second
            ///
            /// @param Lifetime in seconds
            //-----------------------------------------------------
            void SetLifetime(f32 infLifetime);
            //-----------------------------------------------------
            /// Set Looping
            ///
            /// Set whether the emitter is continuous
            ///
            /// @param Toggle on/off
            //-----------------------------------------------------
            void SetLooping(bool inbLooping);
			//-------------------------------------------------
            /// Sets the UVs used for a particle
            //-------------------------------------------------
            void SetParticleUVs(const Core::Rectangle & insUVs);
            //-------------------------------------------------
            /// @author S Downie
            ///
            /// @param Texture atlas used in conjunction with
            /// atlas Id to set the UVs of the particle
            //-------------------------------------------------
            void SetTextureAtlas(const TextureAtlasCSPtr& in_atlas);
            //-------------------------------------------------
            /// @author S Downie
            ///
            /// @param Texture atlas Id used in conjunction with
            /// atlas to set the UVs of the particle.
            ///
            /// NOTE: Atlas must be set first
            //-------------------------------------------------
            void SetTextureAtlasId(const std::string& in_atlasId);

			//-------------------------------------------------
            /// Has this emitter finished emitting its particles? (EmitOnce only!)
            //-------------------------------------------------
            bool GetIsEmittingFinished() { return mbIsEmittingFinished; }
            
        private:
            //-----------------------------------------------------
            /// Update Particle
            ///
            /// Update an indivdual particle with the affectors
            ///
            /// @param Particle
            /// @param DT
            //-----------------------------------------------------
            void UpdateParticle(Particle& in_particle, f32 infDT);
            //-----------------------------------------------------
            /// Emit
            ///
            /// Emit some new particles by pulling from the dead
            /// pool. Emission will depend on the number of
            /// particles emitted and the shape of the emitter
            ///
			/// @param Particle
            //-----------------------------------------------------
            virtual void Emit(Particle& in_particle) = 0;
			
		protected:
			
			//-----------------------------------------------------
			/// Update Sprite Data
			///
			/// Rebuild the sprite data
			//-----------------------------------------------------
			void UpdateSpriteData(const Core::Vector3& invPos, const Core::Colour & insTintColour, SpriteComponent::SpriteData& outsData,
                                  const Core::Vector3& invRight, const Core::Vector3& invUp, const Core::Vector3& invScale);
            
        protected:
            
            Particle* m_particles;
            std::vector<ParticleAffector*> mAffectors;
            
            f32 mfInitialVelocity;
            f32 mfMinInitialVelocity;
            
            Core::Colour mInitialColour;
            Core::Vector3 mvLastEmissionPos;
            
			Core::Rectangle msParticleUVs;
			
            MaterialCSPtr mpMaterial;
            TextureAtlasCSPtr m_atlas;
            
            u32 mudwMaxNumParticles;
            u32 mudwMaxNumParticlesPerEmission;
            u32 mudwNumUsed;
            
            f32 mfCurrentTime;
            f32 mfLastEmissionTime;
            f32 mfEmissionFreq;
            f32 mfTimeToLive;
            f32 mfEnergyLoss;
            Core::Vector2 mvInitialScale;
            
            bool mbShouldLoop;
            bool mbIsEmitting;
            u32 mudwBurstCounter;
            bool mbIsEmittingFinished;
            bool mbIsGlobalSpace;
            
            ParticleComponent* mpOwningComponent;
        };
    }
}

#endif