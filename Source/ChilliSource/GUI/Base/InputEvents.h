//
//  InputEvents.h
//  moFloTest
//
//  Created by Scott Downie on 16/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_INPUT_EVENTS_H_
#define _MO_FLO_GUI_INPUT_EVENTS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Core/Event/GenericEvent.h>

namespace ChilliSource
{
    namespace GUI
    {
        typedef std::function<void(GUIView*, const Input::TouchInfo&)> GUIEventDelegate;
        
        class InputEvents
        {
        public:
            //---User action delegates
            //-----------------------------------------------------------
            /// Get Pressed Inside Event
            ///
            /// A event that is triggered when input is started within
            /// the bounds of the view
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IEvent<GUIEventDelegate>& GetPressedInsideEvent();
            //-----------------------------------------------------------
            /// Get Released Inside Event
            ///
            /// A event that is triggered when input is ended within
            /// the bounds of the view
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IEvent<GUIEventDelegate>& GetReleasedInsideEvent();
            //-----------------------------------------------------------
            /// Get Released Outside Event
            ///
            /// A event that is triggered when input is ended outwith
            /// the bounds of the view having started within it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IEvent<GUIEventDelegate>& GetReleasedOutsideEvent();
            //-----------------------------------------------------------
            /// Get Moved Outside Event
            ///
            /// A event that is triggered when input is registered outwith
            /// the bounds of the view having started within it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IEvent<GUIEventDelegate>& GetMovedOutsideEvent();
            //-----------------------------------------------------------
            /// Get Moved Inside Event
            ///
            /// A event that is triggered when input is detected within
            /// the bounds of the view having started outwith it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IEvent<GUIEventDelegate>& GetMovedInsideEvent();
            //-----------------------------------------------------------
            /// Get Moved Within Event
            ///
            /// A event that is triggered when input is detected within
            /// the bounds of the view having started within it
            ///
            /// @return Event
            //-----------------------------------------------------------
            Core::IEvent<GUIEventDelegate>& GetMovedWithinEvent();
            
            //---Touch Delegates
            //-----------------------------------------------------------
            /// On Touch Began
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            /// @return Whether the view contains the touch
            //-----------------------------------------------------------
            bool OnTouchBegan(GUIView* inpView, const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Moved
            ///
            /// Called when the window receives cursor/touch input
            ///
            /// @param Touch data
            /// @return Whether the view contains the touch
            //-----------------------------------------------------------
            bool OnTouchMoved(GUIView* inpView, const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Touch Ended
            ///
            /// Called when the window stops receiving cursor/touch input
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void OnTouchEnded(GUIView* inpView, const Input::TouchInfo & insTouchInfo);
            
        protected:
            
            //---View events
            Core::CEvent2<GUIEventDelegate> mTouchPressedInside;
            Core::CEvent2<GUIEventDelegate> mTouchReleasedInside;
            Core::CEvent2<GUIEventDelegate> mTouchReleasedOutside;
            Core::CEvent2<GUIEventDelegate> mTouchMoveEnter;
            Core::CEvent2<GUIEventDelegate> mTouchMoveExit;
            Core::CEvent2<GUIEventDelegate> mTouchMovedWithin;
            
        private: 
            
            std::vector<u32> mOpenTouches;  
        };
    }
}

#endif