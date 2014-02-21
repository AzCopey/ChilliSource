/*
 *  GameCentre.h
 *  moFloTest
 *
 *  Created by Scott Downie on 01/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_IOS_PLATFORM_GAME_CENTRE_H_
#define _MO_FLO_IOS_PLATFORM_GAME_CENTRE_H_

#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/Container/WorkerQueue.h>
#include <ChilliSource/Core/String/UTF8String.h>

#include <thread>

@class NSArray;

namespace moFlo
{
	namespace iOSPlatform
	{
		typedef u32 GameCentreRequestID;
		
        //---Flags
		enum class PlayerScope
		{
            k_global,
            k_friends
		};
		enum class TimeScope
		{
            k_allTime,
            k_allToday,
            k_allWeek
		};
        
        //---Containers
		struct LeaderboardsInfo
		{
			DYNAMIC_ARRAY<UTF8String> mNames;
			DYNAMIC_ARRAY<std::string> mCategoryIDs;
		};
        struct ScoreInfo
        {
            UTF8String strPlayerName;       //The players game center alias
            UTF8String strFormattedScore;   //The score correctly formatted according to iTunes leaderboard
            u64 uddwScore;                          //The raw score value
            bool bIsFriend;                         //Whether the player is a friend of the local player
        };
        typedef DYNAMIC_ARRAY<ScoreInfo> ScoreContainer;
        
		struct LeaderboardScoreResults
		{
			std::string strCategoryName;
			ScoreContainer aScores;
		};
		
		//---Request Types
		class IGameCentreRequest;
		class CGameCentrePostScoreRequest;
		class CGameCentreFriendInfoRequest;
		class CGameCentreGetScoresRequest;
		class CGameCentreLeaderboardsInfoRequest;
        class CGameCentreGetNamesRequest;
		
		class CGameCentreUpdateAchievementRequest;
		class CGameCentreAchievementsStatusRequest;
		class CGameCentreAchievementDescriptionsRequest;
		
		class CGameCentreSystem : public Core::ISystem
		{
		public:
            
            //---Callbacks
            typedef fastdelegate::FastDelegate1<const LeaderboardsInfo&> LeaderboardsInfoRequestEventDelegate;
            typedef fastdelegate::FastDelegate1<bool> AuthenticationChangedDelegate;
            typedef fastdelegate::FastDelegate1<const LeaderboardScoreResults&> LeaderboardScoreRequestDelegate;
            typedef fastdelegate::FastDelegate1<const std::string&> RequestErrorDelegate;
			typedef fastdelegate::FastDelegate1<bool> AchievementsResetDelegate;
            
			DECLARE_NAMED_INTERFACE(CGameCentreSystem);
			CGameCentreSystem(bool inbUseTurnBasedMultiplayer = false);
			virtual ~CGameCentreSystem();
			
			//--------------------------------------------------------------------
			/// Is Supported
			///
			/// @return Whether the device supports game centre
			//--------------------------------------------------------------------
			static bool IsSupported(bool inbUseTurnBasedMultiplayer = false);
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //--------------------------------------------------------------------
            /// Get Is Client Authenticated
            ///
            /// Returns true if the client is authenticated, or false if not.
            /// @return true if client authenticated. false otherwise.
            //--------------------------------------------------------------------
            bool GetIsClientAuthenticated() const;
			//--------------------------------------------------------------------
			/// Authenticate Client
			///
			/// Check whether the local player is signed-in/registered and
			/// presents a user interface otherwise
			//--------------------------------------------------------------------
			void AuthenticateClient();
			//--------------------------------------------------------------------
			/// Get Local Player ID
			///
			/// @return String identifier for local client
			//--------------------------------------------------------------------
			const UTF8String& GetLocalPlayerID() const;
			//--------------------------------------------------------------------
			/// Get Local Player Username
			///
			/// @return Username for local client
			//--------------------------------------------------------------------
			const UTF8String& GetLocalPlayerUsername() const;
			
			//---------------------------------------------------------------------
			/// OnAuthenticationChangedEvent
			///
			/// Raised when the game centre user changes (new signin/signout)
			//---------------------------------------------------------------------
			IEvent<AuthenticationChangedDelegate> & OnAuthenticationChangedEvent();
			
			///////////////////////////////////////////////////////////////////////////
			// LEADERBOARDS
			///////////////////////////////////////////////////////////////////////////
			
			//--------------------------------------------------------------------
			/// ShowSystemLeaderboardUI
			///
			/// Show the system leaderboard UI
			//--------------------------------------------------------------------
			void ShowSystemLeaderboardUI();
			
			//--------------------------------------------------------------------
			/// Request Leaderboard Names and IDs
			///
			/// Will form a game center request to retreive the name and 
			/// category identifiers of this app's leaderboards
			//--------------------------------------------------------------------
			void RequestLeaderboardNamesandIDs(const LeaderboardsInfoRequestEventDelegate& inCallback, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate);

			//--------------------------------------------------------------------
			/// Request Scores Within Range
			///
			/// Request that a set of scores from the given leaderboard are
			/// downloaded. Successful download will trigger a callback.
			///
			/// @param Category ID of the board as specified on iTunes Connect
			/// @param Player scope (i.e. global, friends)
			/// @param Time scope (i.e. all time, the last week)
			/// @param Minimum range (position on leaderboard)
			/// @param Maximum range (position on leaderboard)
			/// @param A delegate invoked when the request completes
			//---------------------------------------------------------------------
			void RequestScoresWithinRange(const std::string& instrCategoryID, PlayerScope inePlayerScope, TimeScope ineTimeScope, u32 inudwMinRange, u32 inudwMaxRange, 
                                          LeaderboardScoreRequestDelegate inpDelegate, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate);
			//--------------------------------------------------------------------
			/// Post Score To Leaderboard
			///
			/// Will attempt to send the score to Apple and will cache and
			/// resend if it fails
			///
			/// @param Category ID of the board as specified on iTunes Connect
			/// @param Score
			//--------------------------------------------------------------------
			void PostScoreToLeaderboard(const std::string& instrCategoryID, u64 inuddwScore, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate);
			
			//---------------------------------------------------------------------
			/// Request Friends Information
			///
			/// Request that the server give us the data about the user's friends
			//---------------------------------------------------------------------
			void RequestFriendsInformation(const std::string& instrTag, RequestErrorDelegate inpErrorDelegate);
			
			///////////////////////////////////////////////////////////////////////////
			// ACHIEVEMENTS
			///////////////////////////////////////////////////////////////////////////
			
			//---------------------------------------------------------------------
			///Pushes a view controller showing the system achievements UI
			//---------------------------------------------------------------------
			void ShowSystemAchievementsUI();
            
			//---------------------------------------------------------------------
			/// Updates the current signed-in user's progression in the specified achievement
			/// @param the string Achievement ID of the achievement per iTunes Connect
			/// @param the progression from 0 - 100 towards this achievement. Calling this with 0 will reveal an achievement set to hidden
			//---------------------------------------------------------------------
			void SetAchievementProgression(const std::string & instrID, double inffProgression,bool inbShowToast, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate);
			
			//---------------------------------------------------------------------
			///Resets all achievements
			//---------------------------------------------------------------------
			void ResetAllAchievements();
			//---------------------------------------------------------------------
			/// Checks if the passed in achievement ID has been unlocked
			/// @param the string Achievement ID of the achievement per iTunes Connect
            /// @return bool
			//---------------------------------------------------------------------
			bool IsAchievementUnlocked(const std::string & instrID);
            
			//---------------------------------------------------------------------
			///Subscribe to achievement reset event
			//---------------------------------------------------------------------
			IEvent<AchievementsResetDelegate>& OnAchievementResetEvent();
			
		private:
            //--------------------------------------------------------------------
            /// Add Request To Queue
            ///
            /// Push the request to the back of the queue so that the game
            /// center thread can process it. This is a thread safe method
            ///
            /// @param Game Centre Request
            //--------------------------------------------------------------------
            void AddRequestToQueue(IGameCentreRequest* inpRequest);
            //--------------------------------------------------------------------
            /// Request Names For IDs
            ///
            /// Request a set of player names from the array of player ID's
            ///
            /// @param Array containing player IDs
            /// @param Key to marry the name data to the score data
            //---------------------------------------------------------------------
            void RequestNamesForIDs(NSArray* inpPlayerIDs, const std::string& instrKey, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate);
			//---------------------------------------------------------------------
			/// Process Request Queue
			///
			/// A run loop that handles the queued requests.
			//---------------------------------------------------------------------
			void ProcessRequestQueue();
			//---------------------------------------------------------------------
			/// Process Post Score Request
			///
			/// Unpack the data from the request and attempt to send to game centre
			///
			/// @param Post score request
			//---------------------------------------------------------------------
			void ProcessPostScoreRequest(CGameCentrePostScoreRequest* pRequest);
			//---------------------------------------------------------------------
			/// Process Get Scores Request
			///
			/// Retrieve the scores with the parameters of the request
			///
			/// @param Get scores request
			//---------------------------------------------------------------------
			void ProcessGetScoresRequest(CGameCentreGetScoresRequest* pRequest);
			//---------------------------------------------------------------------
			/// Process Friend Identifiers Request
			///
			/// Send a request for the server to populate the local players friend
			/// array
			///
			/// @param Request
			//---------------------------------------------------------------------
			void ProcessFriendIdentifiersRequest(CGameCentreFriendInfoRequest* pRequest);
			//---------------------------------------------------------------------
			/// Process Leaderboard Info Request
			///
			/// Send a request for the server to get the names and identifiers
			/// of the apps leaderboards.
			///
			/// @param Request
			//---------------------------------------------------------------------
			void ProcessLeaderboardsInfoRequest(CGameCentreLeaderboardsInfoRequest* pRequest);
            //---------------------------------------------------------------------
            /// Process Get Names Request
            ///
            /// Retrieve the names from the given user IDs
            ///
            /// @param Get names request
            //---------------------------------------------------------------------
            void ProcessGetNamesRequest(CGameCentreGetNamesRequest* pRequest);
			

			void ProcessUpdateAchievementRequest(CGameCentreUpdateAchievementRequest * pRequest);
			void ProcessAchievementDescriptionsRequest(CGameCentreAchievementDescriptionsRequest * pRequest);			
			void ProcessAchievementsStatusRequest(CGameCentreAchievementsStatusRequest * pRequest);
			void HandleAchievementStatusArray(NSArray * inpAchievements);
			
			//----------------------------------------------------------------------
			/// On Leaderboard Info Request Complete
			///
			/// The leaderboard names and ID's have been downloaded. Let's unpack 
			/// and make them available for use
			//----------------------------------------------------------------------
			void OnLeaderboardsInfoRequestComplete(NSArray* pCategories, NSArray* pTitles);
			//---------------------------------------------------------------------
			/// On Local Authentication Changed
			///
			/// The user may have disconnected/connected or logged in or out
			//---------------------------------------------------------------------
			void OnLocalAuthenticationChanged();
            //---------------------------------------------------------------------
            /// On Score Range Request Complete
            ///
            /// The score objects have been downloaded but do not contain user-
            /// names. We can use the user ID's to download the names and then 
            /// return the object as a whole
            ///
            /// @param Array of score objects
            /// @param Name object which we use as the key to marry the data
            //---------------------------------------------------------------------
            void OnScoreRangeRequestComplete(NSArray* inpScores, const std::string& instrKey);
            //---------------------------------------------------------------------
            /// On Name Request Complete
            ///
            /// The player objects have been downloaded. We can now combine them
            /// with the score request data and return names and scores 
            /// together
            ///
            /// @param Array of player objects
            /// @param Key to find the downloaded scores
            //---------------------------------------------------------------------
            void OnNameRequestComplete(NSArray* inpPlayers, const std::string& instrKey);
			
		private:
			
            std::thread mRequestThread;
            Core::WorkerQueue<IGameCentreRequest*> mRequestQueue;
			
			LeaderboardsInfo mLeaderboardsInfo;
            
            HASH_MAP<std::string, LeaderboardScoreResults> mMapNameRequestToLeaderboardScoreResults;
			HASH_MAP<std::string, LeaderboardScoreRequestDelegate> mMapRequestNameToRequestDelegate;
			CEvent1<AuthenticationChangedDelegate> mAuthenticationChangedEvent;
			CEvent1<LeaderboardsInfoRequestEventDelegate> mLeaderboardsInfoRequestEvent;
			CEvent1<AchievementsResetDelegate> mAchievementsResetEvent;
			
			bool mbIsGameCentreSupported;
			bool mbProcessRequests;

			UTF8String mstrPlayerID;
			UTF8String mstrPlayerName;
            
			DYNAMIC_ARRAY<std::string> mastrCompletedAchievements;
			NSArray * mpAchievementDescriptions;//<Cheeky little NSArray of GKAchivementDescription objects
			
            bool mbUseTurnBasedMultiplayer;     // true if we want to use turn-based multiplayer, otherwise false.
            u32 mudwNumOpenRequests;
		};
		
        typedef SHARED_PTR<CGameCentreSystem> GameCentreSystemPtr;
	}
}
#endif