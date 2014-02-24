//
//  VirtualKeyboard.h
//  moFlow
//
//  Created by Scott Downie on 26/11/2010
//  Copyright ©2010 Tag Games Limited. All rights reserved
//

#ifndef _MO_FLO_INPUT_VIRTUAL_KEYBOARD_H_
#define _MO_FLO_INPUT_VIRTUAL_KEYBOARD_H_

#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/String/UTF8String.h>

namespace ChilliSource
{
	namespace Input
	{
		typedef fastdelegate::FastDelegate0<> KeyboardEventDelegate;
		typedef fastdelegate::FastDelegate2<const Core::UTF8String&, bool*> TextInputEventDelegate;
        
        enum class KeyboardType
		{
        	k_text,
			k_numeric
		};
		
        //-------------------------------------------------------
        /// Keyboard Capitalisation
        ///
        /// An enum describing the various methods of keyboard
        /// capitalisation that can be used.
        ///
        ///  - KEYBOARD_CAPS_NONE will not capitalise any thing.
        ///  - KEYBOARD_CAPS_WORDS will capitalise the first letter
        ///	   of each word.
        ///  - KEYBOARD_CAPS_SENTENCES will capitalise the first letter
        ///	   of each sentence.
        ///  - KEYBOARD_CAPS_ALL will capitalise every letter.
        //-------------------------------------------------------
        enum class KeyboardCapitalisation
        {
            k_none,
            k_words,
            k_sentences,
            k_all
        };
        
		class IVirtualKeyboard 
		{
		public:
			IVirtualKeyboard() : mbIsActive(false){}
			virtual ~IVirtualKeyboard(){}
			
			virtual void Show() = 0;
			virtual void Hide() = 0;
			
			const Core::UTF8String& GetKeyboardText() const{return mstrText;}
            
            virtual void SetText(const Core::UTF8String& instrText) = 0;
            
            virtual void SetKeyboardType(KeyboardType ineKeyboardType) = 0;
            virtual void SetCapitalisationMethod(KeyboardCapitalisation ineKeyboardCapitalisation) = 0;
			
			Core::IEvent<KeyboardEventDelegate>& GetKeyboardShowEvent() {return mOnKeyboardShowEvent;}
			Core::IEvent<KeyboardEventDelegate>& GetKeyboardHideEvent() {return mOnKeyboardHideEvent;}
			Core::IEvent<TextInputEventDelegate>& GetKeyboardTextChangeEvent() {return mOnKeyboardTextChangeEvent;}
			
			bool IsActive() {return mbIsActive;}
			
		protected:
			
			Core::CEvent0<KeyboardEventDelegate> mOnKeyboardShowEvent;
			Core::CEvent0<KeyboardEventDelegate> mOnKeyboardHideEvent;
			Core::CEvent2<TextInputEventDelegate> mOnKeyboardTextChangeEvent;
			
			Core::UTF8String mstrText;
			
			bool mbIsActive;
		};
	}
}

#endif


