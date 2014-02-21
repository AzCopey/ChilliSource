//
//  KeyboardJavaInterface.cpp
//  moFlow
//
//  Created by Ian Copland on 04/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Platform/Android/Input/KeyboardJavaInterface.h>

#include <jni.h>

#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>


//-----------------------------------------------
/// C function declarations
//-----------------------------------------------
extern "C"
{
	void Java_com_taggames_moflow_input_CKeyboardNativeInterface_NativeOnTextAdded(JNIEnv* inpEnv, jobject inThis, jstring injstrText);
	void Java_com_taggames_moflow_input_CKeyboardNativeInterface_NativeOnTextDeleted(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_input_CKeyboardNativeInterface_NativeOnKeyboardDismissed(JNIEnv* inpEnv, jobject inThis);
}
//-----------------------------------------------
/// Native On Text Added
///
/// Called from java when new keyboard text has
/// been received to be added.
///
/// @param The jni environment.
/// @param The java object calling the function.
/// @param The additional text.
//-----------------------------------------------
void Java_com_taggames_moflow_input_CKeyboardNativeInterface_NativeOnTextAdded(JNIEnv* inpEnv, jobject inThis, jstring injstrText)
{
	moFlo::AndroidPlatform::KeyboardJavaInterfacePtr pKeyboardJI = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CKeyboardJavaInterface>();
	if (pKeyboardJI != NULL)
	{
		moFlo::UTF8String strText = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateUTF8StringFromJString(injstrText);
		moFlo::Task1<const moFlo::UTF8String&> task(pKeyboardJI.get(), &moFlo::AndroidPlatform::CKeyboardJavaInterface::OnTextAdded, strText);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(task);
	}
	inpEnv->DeleteLocalRef(injstrText);
}
//-----------------------------------------------
/// Native On Text Deleted
///
/// Called from java when a keyboard text deletion
/// event is received.
///
/// @param The jni environment.
/// @param The java object calling the function.
//-----------------------------------------------
void Java_com_taggames_moflow_input_CKeyboardNativeInterface_NativeOnTextDeleted(JNIEnv* inpEnv, jobject inThis)
{
	moFlo::AndroidPlatform::KeyboardJavaInterfacePtr pKeyboardJI = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CKeyboardJavaInterface>();
	if (pKeyboardJI != NULL)
	{
		moFlo::Task0 task(pKeyboardJI.get(), &moFlo::AndroidPlatform::CKeyboardJavaInterface::OnTextDeleted);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(task);
	}
}
//-----------------------------------------------
/// Native On Keyboard Dismissed
///
/// Called from java when the keyboard is dismissed.
///
/// @param The jni environment.
/// @param The java object calling the function.
//-----------------------------------------------
void Java_com_taggames_moflow_input_CKeyboardNativeInterface_NativeOnKeyboardDismissed(JNIEnv* inpEnv, jobject inThis)
{
	moFlo::AndroidPlatform::KeyboardJavaInterfacePtr pKeyboardJI = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CKeyboardJavaInterface>();
	if (pKeyboardJI != NULL)
	{
		moFlo::Task0 task(pKeyboardJI.get(), &moFlo::AndroidPlatform::CKeyboardJavaInterface::OnKeyboardDismissed);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(task);
	}
}

namespace moFlo
{
	namespace AndroidPlatform
	{
		namespace
		{
			//-----------------------------------------------
			/// Keyboard Type To Integer
			///
			/// Converts from the keyboard type enum to an
			/// integer that can be passed on to the Java
			/// side of the engine.
			///
			/// @param The keyboard type to convert.
			/// @return The keyboard type in integer form.
			//-----------------------------------------------
			s32 KeyboardTypeToInteger(Input::KeyboardType ineKeyboardType)
			{
				switch (ineKeyboardType)
				{
				case Input::KEYBOARD_TEXT:
					return 0;
				case Input::KEYBOARD_NUMERIC:
					return 1;
				default:
					ERROR_LOG("Invalid keyboard type, cannot be converted.");
					return 0;
				}
			}
			//-----------------------------------------------
			/// Keyboard Capitalisation To Integer
			///
			/// Converts from the Keyboard Capitalisation enum
			/// to an integer that can be passed on to the
			/// Java side of the engine.
			///
			/// @param The Keyboard Capitalisation to convert.
			/// @return The Keyboard Capitalisation in integer
			/// form.
			//-----------------------------------------------
			s32 KeyboardCapitalisationToInteger(Input::KeyboardCapitalisation ineKeyboardCapitalisation)
			{
				switch (ineKeyboardCapitalisation)
				{
				case Input::KEYBOARD_CAPS_NONE:
					return 0;
				case Input::KEYBOARD_CAPS_SENTENCES:
					return 1;
				case Input::KEYBOARD_CAPS_WORDS:
					return 2;
				case Input::KEYBOARD_CAPS_ALL:
					return 3;
				default:
					ERROR_LOG("Invalid keyboard capitalisation, cannot be converted.");
					return 0;
				}
			}
		}

		DEFINE_NAMED_INTERFACE(CKeyboardJavaInterface);
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		CKeyboardJavaInterface::CKeyboardJavaInterface()
		{
			CreateNativeInterface("com/taggames/moflow/input/CKeyboardNativeInterface");
			CreateMethodReference("Activate", "()V");
			CreateMethodReference("Deactivate", "()V");
			CreateMethodReference("SetKeyboardType", "(I)V");
			CreateMethodReference("SetCapitalisationMethod", "(I)V");
		}
		//-----------------------------------------------
		/// Is A
		//-----------------------------------------------
		bool CKeyboardJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (CKeyboardJavaInterface::InterfaceID == inInterfaceID);
		}
		//-----------------------------------------------
		/// Set Text Added Delegate
		//-----------------------------------------------
		void CKeyboardJavaInterface::SetTextAddedDelegate(const TextAddedDelegate& inDelegate)
		{
			mTextAddedDelegate = inDelegate;
		}
		//-----------------------------------------------
		/// Set Text Deleted Delegate
		//-----------------------------------------------
		void CKeyboardJavaInterface::SetTextDeletedDelegate(const TextDeletedDelegate& inDelegate)
		{
			mTextDeletedDelegate = inDelegate;
		}
		//-----------------------------------------------
		/// Set Keyboard Dismissed Delegate
		//-----------------------------------------------
		void CKeyboardJavaInterface::SetKeyboardDismissedDelegate(const KeyboardDismissedDelegate& inDelegate)
		{
			mKeyboardDismissedDelegate = inDelegate;
		}
		//-----------------------------------------------
		/// Activate
		//-----------------------------------------------
		void CKeyboardJavaInterface::Activate()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Activate"));
		}
		//-----------------------------------------------
		/// Deactivate
		//-----------------------------------------------
		void CKeyboardJavaInterface::Deactivate()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Deactivate"));
		}
		//-------------------------------------------
		/// Set Keyboard Type
		//-------------------------------------------
        void CKeyboardJavaInterface::SetKeyboardType(Input::KeyboardType ineKeyboardType)
        {
        	JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	s32 dwKeyboardType = KeyboardTypeToInteger(ineKeyboardType);
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SetKeyboardType"), dwKeyboardType);
        }
		//-------------------------------------------
		/// Set Capitalisation Method
		//-------------------------------------------
        void CKeyboardJavaInterface::SetCapitalisationMethod(Input::KeyboardCapitalisation ineKeyboardCapitalisation)
        {
        	JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	s32 dwKeyboardCapitalisation = KeyboardCapitalisationToInteger(ineKeyboardCapitalisation);
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SetCapitalisationMethod"), dwKeyboardCapitalisation);
        }
		//-----------------------------------------------
		/// On Text Added
		//-----------------------------------------------
		void CKeyboardJavaInterface::OnTextAdded(const UTF8String& instrText)
		{
			if (mTextAddedDelegate != NULL)
			{
				mTextAddedDelegate(instrText);
			}
		}
		//-----------------------------------------------
		/// On Text Deleted
		//-----------------------------------------------
		void CKeyboardJavaInterface::OnTextDeleted()
		{
			if (mTextDeletedDelegate != NULL)
			{
				mTextDeletedDelegate();
			}
		}
		//-----------------------------------------------
		/// On Keyboard Dismissed
		//-----------------------------------------------
		void CKeyboardJavaInterface::OnKeyboardDismissed()
		{
			if (mKeyboardDismissedDelegate != NULL)
			{
				mKeyboardDismissedDelegate();
			}
		}
	}
}