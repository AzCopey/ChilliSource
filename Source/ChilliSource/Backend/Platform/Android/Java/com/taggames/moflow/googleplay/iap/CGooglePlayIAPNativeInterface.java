/**
 * CIAPSystemNativeInterface.java
 * moFlo
 *
 * Created by Scott Downie on 12/06/2013
 * Copyright (c)2013 Tag Games Limited - All rights reserved
 */

package com.taggames.moflow.googleplay.iap;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
import android.util.Log;

import com.taggames.moflow.core.CInterfaceIDType;
import com.taggames.moflow.core.IActivityResults;
import com.taggames.moflow.core.IDestroyable;
import com.taggames.moflow.nativeinterface.INativeInterface;
import com.taggames.moflow.networking.CIAPProductDescription;
import com.taggames.moflow.networking.CIAPTransactionDescription;


public class CGooglePlayIAPNativeInterface  extends INativeInterface implements IDestroyable, IActivityResults
{
	//--------------------------------------------------------------
	/// Member data
	//--------------------------------------------------------------
	public static CInterfaceIDType InterfaceID = new CInterfaceIDType("CGooglePlayIAPNativeInterface");
	
	private IabHelper mIABHelper = null;
	private boolean mbCancelProductDescRequest = false;
	private boolean mbIsPurchasingEnabled = false;
	private boolean mbRequestDescriptionsInProgress = false;
	private List<Purchase> mCurrentPendingTransactions = new ArrayList<Purchase>();
	private Inventory mInventory = null;
	
	//---------------------------------------------------------------------
	/// Native Methods
	//---------------------------------------------------------------------
	public native void NativeOnProductsDescriptionsRequestComplete(String[] inIDs, String[] inNames, String[] inDescriptions, String[] inFormattedPrices);
	public native void NativeOnTransactionStatusUpdated(int inResult, String inProductID, String inTransactionID, String inReceipt);
	public native void NativeOnTransactionClosed(String inProductID, String inTransactionID);
	
	//---------------------------------------------------------------------
	/// Constructor
	//---------------------------------------------------------------------
	public CGooglePlayIAPNativeInterface()
	{
	}
	//---------------------------------------------------------------------
	/// Is A
	///
	/// @param Interface ID
	/// @return Whether the system implements the given interface
	//---------------------------------------------------------------------
	@Override public boolean IsA(CInterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType == InterfaceID);
	}
	//---------------------------------------------------------------------
	/// Init
	///
	/// Prepares and starts the IAB system. This should be the first method
	/// called and it should only be called once. 
	///
	/// @param The public key.
	//---------------------------------------------------------------------
	public void Init(final String instrPublicKey)
	{
		mIABHelper = new IabHelper(mActivity, instrPublicKey);
		mIABHelper.startSetup(new IabHelper.OnIabSetupFinishedListener()
		{
			@Override public void onIabSetupFinished(IabResult result) 
			{
				if(result.isSuccess())
				{
					mbIsPurchasingEnabled = true;
				}
				else
				{
					mbIsPurchasingEnabled = false;
					Log.e("MoFlow", "Cannot setup Google Play IAB: " + result.getMessage());
				}
			}
		});
	}
    //---------------------------------------------------------------
	/// Request Product Descriptions
	///
	/// Request the descriptions of all the products on the store
	///
	/// @param Array of product IDs
	//---------------------------------------------------------------
	public void RequestProductDescriptions(final String[] inProductIDs)
	{
		if(mbRequestDescriptionsInProgress && mbCancelProductDescRequest) // previously requested cancel but new request incoming
		{
			//This this will cause the system to received the previously requested product descriptions. This
			//means it will only work if the new request is identical to the previous one. 
			mbCancelProductDescRequest = false;
			return;
		}
		else if(mbRequestDescriptionsInProgress) // just called twice, bail
			return;
		
		
		if(mInventory != null)
		{
			OnProductsRequestComplete(mInventory, inProductIDs);
			return;
		}
		
		// default flags for new request
		mbCancelProductDescRequest = false;
		mbRequestDescriptionsInProgress = true;
			
		mActivity.runOnUiThread(new Runnable()
		{
			@Override public void run() 
			{
				final List<String> productIDs = new ArrayList<String>(Arrays.asList(inProductIDs));

				mIABHelper.queryInventoryAsync(true, productIDs, new IabHelper.QueryInventoryFinishedListener() 
				{
					@Override public void onQueryInventoryFinished(IabResult result, Inventory inventory) 
					{
						mbRequestDescriptionsInProgress = false;
						
						if(mbCancelProductDescRequest)
							return;
						
						mInventory = inventory;
						
						if(result.isFailure())
						{
							Log.e("MoFlow", "Cannot query Google IAB inventory: " + result.getMessage());
							OnProductsRequestComplete(null, inProductIDs);
							return;
						}

						OnProductsRequestComplete(mInventory, inProductIDs);
					}
				});
			}
		});
    }
    //---------------------------------------------------------------
    /// On Products Request Complete
    ///
    /// Triggered when the products description request completes
    ///
    /// @param Inventory
	/// @param Product IDs
    //---------------------------------------------------------------
    public void OnProductsRequestComplete(Inventory inInventory, final String[] inProductIDs) 
    {
		List<CIAPProductDescription> aResults = new ArrayList<CIAPProductDescription>();
		
		if(mInventory != null)
		{
			for(int i=0; i<inProductIDs.length; ++i)
			{
				SkuDetails details = inInventory.getSkuDetails(inProductIDs[i]);
				if(details != null)
				{
					CIAPProductDescription desc = new CIAPProductDescription();
					desc.ID = details.getSku();
					desc.Name = details.getTitle();
					desc.Description = details.getDescription();
					desc.FormattedPrice = details.getPrice();

					aResults.add(desc);
				}
			}
		}
		
    	String[] ids = new String[aResults.size()];
    	String[] names = new String[aResults.size()];
    	String[] descriptions = new String[aResults.size()];
    	String[] prices = new String[aResults.size()];
    	
    	for(int i=0; i<aResults.size(); ++i)
    	{
    		ids[i] = aResults.get(i).ID;
    		names[i] = aResults.get(i).Name;
    		descriptions[i] = aResults.get(i).Description;
    		prices[i] = aResults.get(i).FormattedPrice;
    	}
  
    	NativeOnProductsDescriptionsRequestComplete(ids, names, descriptions, prices);
    }
    //---------------------------------------------------------------
    /// Cancel Product Descriptions Request
    ///
    /// Prevent the delegate being called with descriptions and
    /// attempt to cancel the pending request
    //---------------------------------------------------------------
    public void CancelProductDescriptionsRequest()
    {
    	mbCancelProductDescRequest = true;
    }
	//---------------------------------------------------------------------
	/// Is Purchasing Enabled
	///
	/// @return whether or not IAB V3 is supported on this device/OS
	//---------------------------------------------------------------------
	public boolean IsPurchasingEnabled()
	{
		return mbIsPurchasingEnabled;
	}
	//---------------------------------------------------------------------
	/// Get Purchase From Inventory
	///
	/// @param Product ID
	/// @return The product if owned by the user or null
	//---------------------------------------------------------------------
	private Purchase GetPurchaseFromInventory(String inProductID)
	{
		if(mInventory == null)
			return null;
		
		return mInventory.getPurchase(inProductID);
	}
	//---------------------------------------------------------------------
	/// Request Product Purchase
	///
	/// Start a transaction of an object with the given product ID.
	/// The transaction listener will receive updates on the transaction
	/// status
	///
	/// @param Product ID
	/// @param Type (managed = 0 or unmanaged = 1 (from java interface))
	//---------------------------------------------------------------------
	public void RequestProductPurchase(final String inProductID, final int inType)
	{
		mActivity.runOnUiThread(new Runnable()
		{
			@Override public void run() 
			{
				IabHelper.OnIabPurchaseFinishedListener listener = new IabHelper.OnIabPurchaseFinishedListener()
				{
					@Override public void onIabPurchaseFinished(IabResult result, Purchase info) 
					{
						int resultStatus = CIAPTransactionDescription.SUCCEEDED;

						if(result.isFailure())
						{
							Log.e("MoFlow", "Google IAB purchase error: " + result.getMessage() + " for product: " + inProductID);

							switch(result.getResponse())
							{
							case IabHelper.BILLING_RESPONSE_RESULT_USER_CANCELED:
							case IabHelper.IABHELPER_USER_CANCELLED:
								resultStatus = CIAPTransactionDescription.CANCELLED;
								break;
							case IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED:
								resultStatus = CIAPTransactionDescription.FAILED;
								info = GetPurchaseFromInventory(inProductID);

								if(info != null)
								{
									//If this is a managed purchase then we are simply restoring the item
									if(info.getDeveloperPayload().compareTo("managed") == 0)
										resultStatus = CIAPTransactionDescription.RESTORED;
									//If this is unmanaged then we are resuming an incomplete purchase
									else
										resultStatus = CIAPTransactionDescription.RESUMED;
								}

								break;
							default:
								resultStatus = CIAPTransactionDescription.FAILED;
								break;
							}
						}

						OnTransactionStatusUpdated(resultStatus, info, inProductID);
					}
				};
				
				//This uses an arbitrary value because the Google docs tell me to. The value represents the request code
				final int MANAGED = 0;
				final int UNMANAGED = 1;
				
				String payload = "unmanaged";
				switch(inType)
				{
					case MANAGED:
						payload = "managed";
						break;
					case UNMANAGED:
						payload = "unmanaged";
						break;
				}
				
				mIABHelper.launchPurchaseFlow(mActivity, inProductID, 250102680, listener, payload);
			}
		});
	}
	//---------------------------------------------------------------------
	/// On Transaction Status Updated
	///
	/// Is triggered whenever the status of a transaction changes i.e.
	/// completes or fails. This can be called at any point once the
	/// listener is attached and can be used to push through older 
	/// outstanding transactions
	///
	/// @param Result
	/// @param Purchase
	/// @param Product ID
	//---------------------------------------------------------------------
	public void OnTransactionStatusUpdated(int inResult, Purchase inPurchase, String inProductID) 
	{
		if(inPurchase != null)
		{
			mCurrentPendingTransactions.add(inPurchase);
			
			JSONObject jsonSig = new JSONObject();
        	try 
        	{
        		jsonSig.put("SignedData", inPurchase.getOriginalJson());
				jsonSig.put("Signature", inPurchase.getSignature());
			}
        	catch (JSONException e) 
			{
				e.printStackTrace();
			}
     
			NativeOnTransactionStatusUpdated(inResult, inProductID, inPurchase.getOrderId(), jsonSig.toString());
		}
		else
		{
			NativeOnTransactionStatusUpdated(inResult, inProductID, "", "");
		}
	}
	//---------------------------------------------------------------------
	/// Close Transaction
	///
	/// Consumes the transaction from the purchase list so that it may
	/// be purchased again
	///
	/// @param Product ID
	/// @param Transaction ID
	//---------------------------------------------------------------------
	public void CloseTransaction(final String inProductID, final String inTransactionID)
	{
		mActivity.runOnUiThread(new Runnable()
		{
			@Override public void run() 
			{
				int count = 0;
				for(Purchase purchase : mCurrentPendingTransactions)
				{
					if(purchase.getOrderId().compareTo(inTransactionID) == 0)
					{
						//Only consume consumables
						if(purchase.getDeveloperPayload().compareTo("unmanaged") == 0)
						{
							mIABHelper.consumeAsync(purchase, new IabHelper.OnConsumeFinishedListener()
							{
								@Override public void onConsumeFinished(Purchase purchase, IabResult result) 
								{
									if(result.isFailure())
									{
										Log.e("MoFlow", "Google IAB consume error: " + result.getMessage() + " for product: " + purchase.getSku());
									}
									
									NativeOnTransactionClosed(inProductID, inTransactionID);
								}
							});
						}

						mCurrentPendingTransactions.remove(count);
						return;
					}

					count++;
				}
				
				NativeOnTransactionClosed(inProductID, inTransactionID);
			}
		});
	}
	//---------------------------------------------------------------------
	/// Restore Pending Managed Transactions
	///
	/// Restore any pending transactions from the last session
	///
	/// @param List of managed pending transactions
	//---------------------------------------------------------------------
	public void RestorePendingManagedTransactions(final String[] inPendingManagedTransactionIDs)
	{
		if(mInventory == null)
			return;
		
		for(String transactionID : inPendingManagedTransactionIDs)
		{
			List<Purchase> purchases = mInventory.getAllPurchases();
			
			Purchase pendingPurchase = null;
			
			for(Purchase purchase : purchases)
			{
				if((purchase.getDeveloperPayload().compareTo("managed") == 0) && (purchase.getOrderId().compareTo(transactionID) == 0))
				{
					pendingPurchase = purchase;
					break;
				}
			}
			
			if(pendingPurchase != null)
			{
				OnTransactionStatusUpdated(CIAPTransactionDescription.RESUMED, pendingPurchase, pendingPurchase.getSku());
			}
		}
	}
	//---------------------------------------------------------------------
	/// Restore Pending Unmanaged Transactions
	///
	/// Restore any pending transactions from the last session
	//---------------------------------------------------------------------
	public void RestorePendingUnmanagedTransactions()
	{
		//Loop through the purchase list and pass on any managed products as new transactions but with the original receipts
		if(mInventory == null)
			return;
		
		List<Purchase> purchases = mInventory.getAllPurchases();
		
		for(Purchase purchase : purchases)
		{
			//If any consumables are in the list then they have no been consumed and are pending
			if(purchase.getDeveloperPayload().compareTo("unmanaged") == 0)
			{
				mCurrentPendingTransactions.add(purchase);

				//Apparently 0 is good, 1 is cancelled and 2 is refunded
				switch(purchase.getPurchaseState())
				{
				case 0:
					OnTransactionStatusUpdated(CIAPTransactionDescription.RESUMED, purchase, purchase.getSku());
					break;
				case 1:
					OnTransactionStatusUpdated(CIAPTransactionDescription.CANCELLED, purchase, purchase.getSku());
					break;
				case 2:
					OnTransactionStatusUpdated(CIAPTransactionDescription.REFUNDED, purchase, purchase.getSku());
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------
	/// Restore Managed Purchases
	///
	/// Restores all previous managed purchase by injecting them as new
	/// transactions
	//---------------------------------------------------------------------
	public void RestoreManagedPurchases()
	{
		//Loop through the purchase list and pass on any managed products as new transactions but with the original receipts
		if(mInventory == null)
		{
			Log.e("moFlow", "IAPSystem: Products must be registered and requested before any other actions");
			throw new NullPointerException("Products must be registered and requested before any other actions");
		}
		
		List<Purchase> purchases = mInventory.getAllPurchases();
		
		for(Purchase purchase : purchases)
		{
			if(purchase.getDeveloperPayload().compareTo("managed") == 0)
			{
				//Apparently 0 is good, 1 is cancelled and 2 is refunded
				switch(purchase.getPurchaseState())
				{
				case 0:
					OnTransactionStatusUpdated(CIAPTransactionDescription.RESTORED, purchase, purchase.getSku());
					break;
				case 1:
					OnTransactionStatusUpdated(CIAPTransactionDescription.CANCELLED, purchase, purchase.getSku());
					break;
				case 2:
					OnTransactionStatusUpdated(CIAPTransactionDescription.REFUNDED, purchase, purchase.getSku());
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------
	/// On Destroy
	///
	/// Called when the moFlow activity is destroyed.
	//---------------------------------------------------------------------
	@Override public void OnDestroy() 
	{
		if(mIABHelper != null)
		{
			mIABHelper.dispose();
			mIABHelper = null;
		}
	}
	//---------------------------------------------------------------------
	/// On Activity Result
	///
	/// Called when the moFlow activity is "onActivityResult" is called.
	//---------------------------------------------------------------------
	@Override public void OnActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(mIABHelper != null)
		{
			mIABHelper.handleActivityResult(requestCode, resultCode, data);
		}
	}
}