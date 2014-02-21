/*
 *  CMoFlowActivity.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow;
import android.app.Activity;
import android.content.Intent;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.taggames.moflow.input.CKeyboardNativeInterface;
import com.taggames.moflow.nativeinterface.CContactInformationProviderNativeInterface;
import com.taggames.moflow.nativeinterface.CCoreNativeInterface;
import com.taggames.moflow.nativeinterface.CHttpConnectionNativeInterface;
import com.taggames.moflow.nativeinterface.CNativeInterfaceManager;
import com.taggames.moflow.nativeinterface.CSMSCompositionNativeInterface;
import com.taggames.moflow.nativeinterface.CSharedPreferencesNativeInterface;
import com.taggames.moflow.nativeinterface.CTwitterAuthenticationViewNativeInterface;
import com.taggames.moflow.nativeinterface.CWebViewNativeInterface;
import com.taggames.moflow.powermanager.CMoFlowPowerManager;

import org.fmod.FMODAudioDevice;

public class CMoFlowActivity extends Activity 
{
	//-----------------------------------------------------------------
	/// Private Static Member Data
	//-----------------------------------------------------------------
	private static CMoFlowActivity mActivity;
	//-----------------------------------------------------------------
	/// Private Member Data
	//-----------------------------------------------------------------
	private RelativeLayout mViewContainer;
	private CMoFlowSurface mSurface;
	private CMoFlowLoadingView mLoadingView;
	private CMoFlowRenderer mRenderer;
	private FMODAudioDevice mFMODAudioDevice = new FMODAudioDevice();
	private boolean mbActivityRunning = false;
	private boolean mbActivityInForeground = false;
	private volatile boolean mbActivityActive = false;
	//-----------------------------------------------------------------
	/// Get Activity
	///
	/// @param MoFlow Activity
	//-----------------------------------------------------------------
	public static CMoFlowActivity GetActivity()
	{
		return mActivity;
	}
	//------------------------------------------------------------------------
	/// On Create
	///
	/// This is called when the activity is first created. In our case this
	/// is the entry point to our application
	///
	/// @param the saved instance state. This is of no use to us.
	//------------------------------------------------------------------------
    @Override public void onCreate(Bundle savedInstanceState) 
    {	
        super.onCreate(savedInstanceState);
        try
        {
        	mActivity = this;
      
        	//go full screen!
        	requestWindowFeature(Window.FEATURE_NO_TITLE);
        	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        	
        	//create the view container
        	mViewContainer = new RelativeLayout(this);
        	setContentView(mViewContainer);
        	
        	mLoadingView = new CMoFlowLoadingView(this);
        	
        	//create the initial surface
        	mRenderer = new CMoFlowRenderer(mLoadingView);
        	mRenderer.NewLaunchIntentReceived();
    		mSurface = new CMoFlowSurface(this, mRenderer);
    		mViewContainer.addView(mSurface);
    		mLoadingView.Present("com_taggames_default");
    		
        	//setup the native interface
        	CNativeInterfaceManager.GetSingleton().Setup(this);
        	
          	//initialise the old style native interfaces (These should be removed over time as each of these is changed over to the new system!)
        	CHttpConnectionNativeInterface.Setup(this);
        	CSharedPreferencesNativeInterface.Setup(this);
        	CSMSCompositionNativeInterface.Setup(this);
        	CContactInformationProviderNativeInterface.Setup(this);
            CWebViewNativeInterface.Setup(this);
            CMoFlowPowerManager.Setup(this);
            CTwitterAuthenticationViewNativeInterface.Setup(this);
        }
        catch (Exception e)
        {
        	android.util.Log.e("MoFlow", "Activity.onCreate has thrown an exception:");
        	e.printStackTrace();
        }
    }
    //------------------------------------------------------------------------
  	/// On Resume
  	///
  	/// This is called whenever the activity becomes the foreground activity.
  	//------------------------------------------------------------------------
    @Override protected void onResume() 
    {
    	super.onResume();
    	
    	if(!mLoadingView.IsPresented())
    	{
    		mLoadingView.Present("com_taggames_resume");
    	}
    	
        mFMODAudioDevice.start();
        mSurface.onResume();
        CNativeInterfaceManager.GetSingleton().OnResume();
        CMoFlowPowerManager.RequestWakeLock(CMoFlowPowerManager.LOCK_TYPE.SCREEN_DIM_LOCK);
        
        mbActivityRunning = true;
        TryChangeActive();
    }
    //------------------------------------------------------------------------
  	/// On Window Focus Changed
  	///
  	/// This is called when window focus changes.
  	//------------------------------------------------------------------------
    @Override public void onWindowFocusChanged(boolean inbHasFocus)
    {
    	mbActivityInForeground = inbHasFocus;
    	TryChangeActive();
    	super.onWindowFocusChanged(inbHasFocus);
    }
  	//------------------------------------------------------------------------
  	/// On Stop
  	///
  	/// This is called whenever the activity is no longer in the foreground
  	//------------------------------------------------------------------------
    @Override public void onPause() 
    {
    	mbActivityRunning = false;
    	TryChangeActive();
    	
    	CNativeInterfaceManager.GetSingleton().OnPause();
    	mSurface.onPause();
    	mFMODAudioDevice.stop();
        CMoFlowPowerManager.ReleaseLock(CMoFlowPowerManager.LOCK_TYPE.SCREEN_DIM_LOCK);
        super.onPause();
    }
    //------------------------------------------------------------------------
  	/// On Stop
  	///
  	/// This is called whenever the activity is no longer visible.
  	//------------------------------------------------------------------------
    @Override public void onStop()
    {
	    super.onStop();
    }
  	//------------------------------------------------------------------------
  	/// On Destroy
  	///
  	/// This is called when the lifecycle of the activty ends. In our case this
    /// is the end point of the application.
  	//------------------------------------------------------------------------
    @Override public void onDestroy()
    {
    	CNativeInterfaceManager.GetSingleton().OnDestroy();
	    super.onDestroy();
    }
    //------------------------------------------------------------------------
    /// On New Intent
    ///
    /// This is triggered whenever the application is launched from
    /// an intent and already exists in the activity stack
    ///
    /// @param Intent
    //------------------------------------------------------------------------
    @Override public void onNewIntent(Intent inIntent)
    {
    	setIntent(inIntent);
    	mRenderer.NewLaunchIntentReceived();
    	super.onNewIntent(inIntent);
    }
    //------------------------------------------------------------------------
  	/// On Activity Result
  	///
  	/// This is called whenever an activity ends
    ///
    /// @param request code 
    /// @param result code
    /// @param data return from intent
  	//------------------------------------------------------------------------
    @Override protected void onActivityResult(int requestCode, int resultCode, Intent data) 
    {
    	CNativeInterfaceManager.GetSingleton().OnActivityResult(requestCode, resultCode, data);
	}
    //------------------------------------------------------------------------
  	/// On Configuration Changed
  	///
  	/// This is called whenever the acivity configuration changes. This is
    /// used to handle things like orientation changes and hardware keyboards
    /// being shown/hidden.
    ///
    /// @param the new config settings.
  	//------------------------------------------------------------------------
    @Override public void onConfigurationChanged(Configuration newConfig) 
    {     
    	super.onConfigurationChanged(newConfig);
    	
    	// Checks whether a hardware keyboard is available    
    	if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_NO) 
    	{   
    		CKeyboardNativeInterface keyboardNI = (CKeyboardNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CKeyboardNativeInterface.InterfaceID);
    		if (keyboardNI != null)
    		{
    			keyboardNI.SetHardwareKeyboardOpen();
    		}
    	} 
    	else if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES) 
    	{
    		CKeyboardNativeInterface keyboardNI = (CKeyboardNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CKeyboardNativeInterface.InterfaceID);
    		if (keyboardNI != null)
    		{
    			keyboardNI.SetHardwareKeyboardClosed();
    		}
    	}
    }
	//------------------------------------------------------------------------
  	/// On Back Pressed
    ///
    /// If no view has consumed the back button pressed event then the event
    /// is passed on to the native side of the application. If it does not 
    /// do anything with it, the event is discarded as a moFlow application
    /// will end up in a broken state if the MoFlow Activity is finalised.
  	//------------------------------------------------------------------------
    @Override public void onBackPressed() 
    {
    	//create the task to be run on the rendering thread
		Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				((CCoreNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CCoreNativeInterface.InterfaceID)).OnBackPressed();
			}
		};
		
		//run the task.
		mSurface.queueEvent(task);
    }
	//------------------------------------------------------------------------
  	/// Try Change Active
  	///
  	/// Tries to change whether or not the activity is active. This will only
    /// occur in two cases. The activity will become active if the activity is
    /// running and it is in the foreground. It will become inactive if the
    /// activity is not running. Otherwise no change will occur.
  	//------------------------------------------------------------------------
    void TryChangeActive()
    {
    	if (mbActivityRunning == true && mbActivityInForeground == true)
    	{
    		mbActivityActive = true;
    	}
    	else if (mbActivityRunning == false)
    	{
    		mbActivityActive = false;
    	}
    }
	//------------------------------------------------------------------------
  	/// Is Active
  	///
  	/// @return whether or not the activity is currently active. By being
    /// active, moFlow should be updating.
  	//------------------------------------------------------------------------
    public boolean IsActive()
    {
    	return mbActivityActive;
    }
	//------------------------------------------------------------------------
  	/// Get View Container
  	///
  	/// @return the view container
  	//------------------------------------------------------------------------
    public RelativeLayout GetViewContainer()
    {
    	return mViewContainer;
    }
	//------------------------------------------------------------------------
  	/// Get Surface
  	///
  	/// @return the opengl Surface
  	//------------------------------------------------------------------------
    public CMoFlowSurface GetSurface()
    {
    	return mSurface;
    }
	//------------------------------------------------------------------
	/// Set Max FPS
	///
	/// @param Max FPS to clamp to
	//------------------------------------------------------------------
	public void SetMaxFPS(int inMaxFPS)
	{
		mRenderer.SetMaxFPS(inMaxFPS);
	}
    //-----------------------------------------------------------------------
    /// Make Toast
    ///
    /// Display a toast notification with the given text
    ///
    /// @param Text
    //-----------------------------------------------------------------------
    public void MakeToast(final String instrText)
    {
        runOnUiThread(new Runnable() 
        {
		    public void run() 
		    {
                Toast.makeText(getApplicationContext(), instrText, Toast.LENGTH_LONG).show();
		    }
		});
    }
    //-----------------------------------------------------------------------
    /// Show System Confirm Dialog
    ///
    /// Display the systems confirmation dialog 
    ///
    /// @param ID that is returned in pressed delegate 
    /// @param Title text
    /// @param Message text
    /// @param Confirm text
    /// @param Cancel text
    //-----------------------------------------------------------------------
    public void ShowSystemConfirmDialog(final int indwDialogID, final String instrTitle, final String instrMessage, final String instrConfirm, final String instrCancel)		
    {
    	final Activity activity = this;
        runOnUiThread(new Runnable() 
        {
        	public void run()
        	{
        		new AlertDialog.Builder(activity)
        		.setIcon(android.R.drawable.ic_dialog_alert)
        		.setTitle(instrTitle)
        		.setMessage(instrMessage)
        		.setPositiveButton(instrConfirm, new DialogInterface.OnClickListener() 
        		{
        			@Override public void onClick(DialogInterface dialog, int which) 
        			{
        				((CCoreNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CCoreNativeInterface.InterfaceID)).OnDialogConfirmed(indwDialogID);
        			}
        		})
        		.setNegativeButton(instrCancel,new DialogInterface.OnClickListener() 
        		{
        			@Override public void onClick(DialogInterface dialog, int which) 
        			{
        				((CCoreNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CCoreNativeInterface.InterfaceID)).OnDialogCancelled(indwDialogID);
        			}
        		})
        		.show();
        	}
        });
    }
    //------------------------------------------------------------------------
    /// Show System Dialog
    ///
    /// Display the systems dialog 
    ///
    /// @param ID that is returned in pressed delegate 
    /// @param Title text
    /// @param Message text
    /// @param Confirm text
    //------------------------------------------------------------------------
    public void ShowSystemDialog(final int indwDialogID, final String instrTitle, final String instrMessage, final String instrConfirm)		
    {
    	final Activity activity = this;
        runOnUiThread(new Runnable() 
        {
        	public void run()
        	{
        		new AlertDialog.Builder(activity)
        		.setIcon(android.R.drawable.ic_dialog_alert)
        		.setTitle(instrTitle)
        		.setMessage(instrMessage)
        		.setPositiveButton(instrConfirm, new DialogInterface.OnClickListener() 
        		{
        			@Override public void onClick(DialogInterface dialog, int which) 
        			{
        				((CCoreNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CCoreNativeInterface.InterfaceID)).OnDialogConfirmed(indwDialogID);
        			}
        		})
        		.show();
        	}
        });
    }    
}