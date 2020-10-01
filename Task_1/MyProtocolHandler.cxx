/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */


#include "ListenerHelper.h"
#include "MyProtocolHandler.h"
#include "WorkMod.h"

#include <com/sun/star/awt/MessageBoxButtons.hpp>
#include <com/sun/star/awt/Toolkit.hpp>
#include <com/sun/star/awt/XMessageBoxFactory.hpp>
#include <com/sun/star/frame/ControlCommand.hpp>
#include <com/sun/star/text/XTextViewCursorSupplier.hpp>
#include <com/sun/star/sheet/XSpreadsheetView.hpp>
#include <com/sun/star/system/SystemShellExecute.hpp>
#include <com/sun/star/system/SystemShellExecuteFlags.hpp>
#include <com/sun/star/system/XSystemShellExecute.hpp>
#include <cppuhelper/supportsservice.hxx>

using namespace com::sun::star::awt;
using namespace com::sun::star::frame;
using namespace com::sun::star::system;
using namespace com::sun::star::uno;

using com::sun::star::beans::NamedValue;
using com::sun::star::beans::PropertyValue;
using com::sun::star::sheet::XSpreadsheetView;
using com::sun::star::text::XTextViewCursorSupplier;
using com::sun::star::util::URL;

ListenerHelper aListenerHelper;

void BaseDispatch::ShowMessageBox( const Reference< XFrame >& rFrame, const ::rtl::OUString& aTitle, const ::rtl::OUString& aMsgText )
{
    if ( !mxToolkit.is() )
        mxToolkit = Toolkit::create(mxContext);
    Reference< XMessageBoxFactory > xMsgBoxFactory( mxToolkit, UNO_QUERY );
    if ( rFrame.is() && xMsgBoxFactory.is() )
    {
        Reference< XMessageBox > xMsgBox = xMsgBoxFactory->createMessageBox(
            Reference< XWindowPeer >( rFrame->getContainerWindow(), UNO_QUERY ),
            com::sun::star::awt::MessageBoxType_INFOBOX,
            MessageBoxButtons::BUTTONS_OK,
            aTitle,
            aMsgText );

        if ( xMsgBox.is() )
            xMsgBox->execute();
    }
}

void BaseDispatch::SendCommand( const com::sun::star::util::URL& aURL, const ::rtl::OUString& rCommand, const Sequence< NamedValue >& rArgs, sal_Bool bEnabled )
{
    Reference < XDispatch > xDispatch =
            aListenerHelper.GetDispatch( mxFrame, aURL.Path );

    FeatureStateEvent aEvent;

    aEvent.FeatureURL = aURL;
    aEvent.Source     = xDispatch;
    aEvent.IsEnabled  = bEnabled;
    aEvent.Requery    = sal_False;

    ControlCommand aCtrlCmd;
    aCtrlCmd.Command   = rCommand;
    aCtrlCmd.Arguments = rArgs;

    aEvent.State <<= aCtrlCmd;
    aListenerHelper.Notify( mxFrame, aEvent.FeatureURL.Path, aEvent );
}

void BaseDispatch::SendCommandTo( const Reference< XStatusListener >& xControl, const URL& aURL, const ::rtl::OUString& rCommand, const Sequence< NamedValue >& rArgs, sal_Bool bEnabled )
{
    FeatureStateEvent aEvent;

    aEvent.FeatureURL = aURL;
    aEvent.Source     = (::com::sun::star::frame::XDispatch*) this;
    aEvent.IsEnabled  = bEnabled;
    aEvent.Requery    = sal_False;

    ControlCommand aCtrlCmd;
    aCtrlCmd.Command   = rCommand;
    aCtrlCmd.Arguments = rArgs;

    aEvent.State <<= aCtrlCmd;
    xControl->statusChanged( aEvent );
}

void SAL_CALL MyProtocolHandler::initialize( const Sequence< Any >& aArguments ) throw ( Exception, RuntimeException)
{
    Reference < XFrame > xFrame;
    if ( aArguments.getLength() )
    {
        // the first Argument is always the Frame, as a ProtocolHandler needs to have access
        // to the context in which it is invoked.
        aArguments[0] >>= xFrame;
        mxFrame = xFrame;
    }
}

Reference< XDispatch > SAL_CALL MyProtocolHandler::queryDispatch(   const URL& aURL, const ::rtl::OUString& sTargetFrameName, sal_Int32 nSearchFlags )
                throw( RuntimeException )
{
    Reference < XDispatch > xRet;
    if ( !mxFrame.is() )
        return 0;

    Reference < XController > xCtrl = mxFrame->getController();
    if ( xCtrl.is() && aURL.Protocol == "vnd.demo.complextoolbarcontrols.demoaddon:" )
    {
        Reference < XTextViewCursorSupplier > xCursor( xCtrl, UNO_QUERY );
        Reference < XSpreadsheetView > xView( xCtrl, UNO_QUERY );
        if ( !xCursor.is() && !xView.is() )
            // without an appropriate corresponding document the handler doesn't function
            return xRet;

        if ( aURL.Path == "ButtonCmd1" ||
             aURL.Path == "ButtonCmd2" ||
             aURL.Path == "ComboboxCmd" ||
             aURL.Path == "ToggleDropdownButtonCmd" ||
             aURL.Path == "DropdownButtonCmd" ||
             aURL.Path == "SpinfieldCmd" ||
             aURL.Path == "EditfieldCmd" ||
             aURL.Path == "DropdownboxCmd" )
        {
            xRet = aListenerHelper.GetDispatch( mxFrame, aURL.Path );
            if ( !xRet.is() )
            {
                xRet = xCursor.is() ? (BaseDispatch*) new WriterDispatch( mxContext, mxFrame ) :
                    (BaseDispatch*) new CalcDispatch( mxContext, mxFrame );
                aListenerHelper.AddDispatch( xRet, mxFrame, aURL.Path );
            }
        }
    }

    return xRet;
}

Sequence < Reference< XDispatch > > SAL_CALL MyProtocolHandler::queryDispatches( const Sequence < DispatchDescriptor >& seqDescripts )
            throw( RuntimeException )
{
    sal_Int32 nCount = seqDescripts.getLength();
    Sequence < Reference < XDispatch > > lDispatcher( nCount );

    for( sal_Int32 i=0; i<nCount; ++i )
        lDispatcher[i] = queryDispatch( seqDescripts[i].FeatureURL, seqDescripts[i].FrameName, seqDescripts[i].SearchFlags );

    return lDispatcher;
}

::rtl::OUString MyProtocolHandler_getImplementationName ()
    throw (RuntimeException)
{
    return ::rtl::OUString( MYPROTOCOLHANDLER_IMPLEMENTATIONNAME );
}

Sequence< ::rtl::OUString > SAL_CALL MyProtocolHandler_getSupportedServiceNames(  )
    throw (RuntimeException)
{
    Sequence < ::rtl::OUString > aRet(1);
    aRet[0] = ::rtl::OUString( MYPROTOCOLHANDLER_SERVICENAME );
    return aRet;
}

#undef SERVICE_NAME

Reference< XInterface > SAL_CALL MyProtocolHandler_createInstance( const Reference< XComponentContext > & rSMgr)
    throw( Exception )
{
    return (cppu::OWeakObject*) new MyProtocolHandler( rSMgr );
}

// XServiceInfo
::rtl::OUString SAL_CALL MyProtocolHandler::getImplementationName(  )
    throw (RuntimeException)
{
    return MyProtocolHandler_getImplementationName();
}

sal_Bool SAL_CALL MyProtocolHandler::supportsService( const ::rtl::OUString& rServiceName )
    throw (RuntimeException)
{
    return cppu::supportsService(this, rServiceName);
}

Sequence< ::rtl::OUString > SAL_CALL MyProtocolHandler::getSupportedServiceNames(  )
    throw (RuntimeException)
{
    return MyProtocolHandler_getSupportedServiceNames();
}

void SAL_CALL BaseDispatch::dispatch( const URL& aURL, const Sequence < PropertyValue >& lArgs ) throw (RuntimeException)
{
    /* Its necessary to hold this object alive, till this method finish.
       May the outside dispatch cache (implemented by the menu/toolbar!)
       forget this instance during de-/activation of frames (focus!).

        E.g. An open db beamer in combination with the My-Dialog
        can force such strange situation :-(
     */
    Reference< XInterface > xSelfHold(static_cast< XDispatch* >(this), UNO_QUERY);

    if ( aURL.Protocol == "vnd.demo.complextoolbarcontrols.demoaddon:" )
    {
        if ( aURL.Path == "ButtonCmd1" )
        {
            // open the LibreOffice web page
            //::rtl::OUString sURL("https://vk.com/just_dy");
            ItsATrans();
            Reference< XSystemShellExecute > xSystemShellExecute(
                SystemShellExecute::create(mxContext) );
            try
            {
                //xSystemShellExecute->execute( sURL, ::rtl::OUString(), SystemShellExecuteFlags::URIS_ONLY );
            }
            catch( Exception& rEx )
            {
                (void)rEx;
            }
        }
        else if( aURL.Path == "ButtonCmd2" )
        {
            // open the LibreOffice web page
            //::rtl::OUString sURL("https://vk.com/yaroslav.konst");
            NewFile();
            Reference< XSystemShellExecute > xSystemShellExecute(
                SystemShellExecute::create(mxContext) );
            try
            {
                //xSystemShellExecute->execute( sURL, ::rtl::OUString(), SystemShellExecuteFlags::URIS_ONLY );
            }
            catch( Exception& rEx )
            {
                (void)rEx;
            }
        }
    }
}

void SAL_CALL BaseDispatch::addStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
    if ( aURL.Protocol == "vnd.demo.complextoolbarcontrols.demoaddon:" )
    {
        if ( aURL.Path == "ButtonCmd" )
        {
            // just enable this command
            ::com::sun::star::frame::FeatureStateEvent aEvent;
            aEvent.FeatureURL = aURL;
            aEvent.Source = (::com::sun::star::frame::XDispatch*) this;
            aEvent.IsEnabled = mbButtonEnabled;
            aEvent.Requery = sal_False;
            aEvent.State = Any();
            xControl->statusChanged( aEvent );
        }

        aListenerHelper.AddListener( mxFrame, xControl, aURL.Path );
    }
}

void SAL_CALL BaseDispatch::removeStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
    aListenerHelper.RemoveListener( mxFrame, xControl, aURL.Path );
}

void SAL_CALL BaseDispatch::controlEvent( const ControlEvent& Event ) throw (RuntimeException)
{
    if ( Event.aURL.Protocol == "vnd.demo.complextoolbarcontrols.demoaddon:" )
    {
        if ( Event.aURL.Path == "ComboboxCmd" )
        {
            // We get notifications whenever the text inside the combobox has been changed.
            // We store the new text into a member.
            if ( Event.Event == "TextChanged" )
            {
                rtl::OUString aNewText;
                sal_Bool      bHasText( sal_False );
                for ( sal_Int32 i = 0; i < Event.aInformation.getLength(); i++ )
                {
                    if ( Event.aInformation[i].Name == "Text" )
                    {
                        bHasText = Event.aInformation[i].Value >>= aNewText;
                        break;
                    }
                }

                if ( bHasText )
                    maComboBoxText = aNewText;
            }
        }
    }
}

BaseDispatch::BaseDispatch( const Reference< XComponentContext > &rxContext,
                            const Reference< XFrame >& xFrame,
                            const ::rtl::OUString& rServiceName )
        : mxContext( rxContext )
        , mxFrame( xFrame )
        , msDocService( rServiceName )
        , mbButtonEnabled( sal_True )
{
}

BaseDispatch::~BaseDispatch()
{
    mxFrame.clear();
    mxContext.clear();
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
