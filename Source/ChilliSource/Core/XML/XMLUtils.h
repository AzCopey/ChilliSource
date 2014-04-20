/** 
 * File: XMLUtils.h
 * Date: 15/11/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_XML_XMLUTILS_H_
#define _MOFLO_CORE_XML_XMLUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/XML/tinyxml.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Core
	{
		class XMLUtils
		{
		public:
			static bool GetAttributeValue(TiXmlElement* inpElement, const std::string & inAttributeName, std::string & inAttributeValue);
		
			//Default template is unspported
			template <typename T> static const T GetAttributeValueOrDefault(TiXmlElement* inpElement, const std::string & inAttributeName, const T & inDefault)
			{
				CS_LOG_WARNING("Called GetAttributeValueOrDefault on an unsupported type");
			}
			
			static TiXmlElement* FirstChildElementWithName(TiXmlElement* inpParentElement, const std::string & inName);
			
			static TiXmlElement* NextSiblingElementWithName(TiXmlElement* inpPreviousElement);
			
		};
		
		template <> const std::string XMLUtils::GetAttributeValueOrDefault<std::string>(TiXmlElement* inpElement, const std::string & inAttributeName, const std::string & inDefault);
		template <> const f32 XMLUtils::GetAttributeValueOrDefault<f32>(TiXmlElement* inpElement, const std::string & inAttributeName, const f32 & inDefault);
		template <> const s32 XMLUtils::GetAttributeValueOrDefault<s32>(TiXmlElement* inpElement, const std::string & inAttributeName, const s32 & inDefault);
		template <> const u32 XMLUtils::GetAttributeValueOrDefault<u32>(TiXmlElement* inpElement, const std::string & inAttributeName, const u32 & inDefault);
		template <> const s64 XMLUtils::GetAttributeValueOrDefault<s64>(TiXmlElement* inpElement, const std::string & inAttributeName, const s64 & inDefault);
		template <> const u64 XMLUtils::GetAttributeValueOrDefault<u64>(TiXmlElement* inpElement, const std::string & inAttributeName, const u64 & inDefault);
		template <> const bool XMLUtils::GetAttributeValueOrDefault<bool>(TiXmlElement* inpElement, const std::string & inAttributeName, const bool & inDefault);		
		template <> const Vector2 XMLUtils::GetAttributeValueOrDefault<Vector2>(TiXmlElement* inpElement, const std::string & inAttributeName, const Vector2 & inDefault);
		template <> const Vector3 XMLUtils::GetAttributeValueOrDefault<Vector3>(TiXmlElement* inpElement, const std::string & inAttributeName, const Vector3 & inDefault);
		template <> const Vector4Old XMLUtils::GetAttributeValueOrDefault<Vector4Old>(TiXmlElement* inpElement, const std::string & inAttributeName, const Vector4Old & inDefault);
		template <> const Matrix4x4Old XMLUtils::GetAttributeValueOrDefault<Matrix4x4Old>(TiXmlElement* inpElement, const std::string & inAttributeName, const Matrix4x4Old & inDefault);
		template <> const QuaternionOld XMLUtils::GetAttributeValueOrDefault<QuaternionOld>(TiXmlElement* inpElement, const std::string & inAttributeName, const QuaternionOld & inDefault);
		template <> const Colour XMLUtils::GetAttributeValueOrDefault<Colour>(TiXmlElement* inpElement, const std::string & inAttributeName, const Colour & inDefault);
	}
}

#endif