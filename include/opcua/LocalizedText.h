#ifndef owa_localizedtext_h
#define owa_localizedtext_h
/*
* ogamma OPC UA C++ SDK
* Copyright(c) One-Way Automation Inc.
* https://onewayautomation.com/opcua-sdk
* All rights reserved.
*/
#include <stdint.h>
#include <string>

namespace OWA {
	namespace OpcUa {
		class LocalizedText
		{
		public:
			LocalizedText()
			{
			}
			LocalizedText(const char* text)
			{
				this->text = text;
			}

			LocalizedText(const std::string& text, const std::string& locale)
			{
				this->locale = locale;
				this->text = text;
			}

			LocalizedText(const char* text, char* locale) {
				this->locale = locale;
				this->text = text;
			}

			LocalizedText(const LocalizedText& other) {
				*this = other;
			}
			~LocalizedText() {}

			LocalizedText& operator=(const LocalizedText& other) {
				if (this != &other) {
					locale = other.locale;
					text = other.text;
				}
				return *this;
			}
			LocalizedText& operator=(const std::string& text_) {
				this->text = text_;
				this->locale = "en";
				return *this;
			}
			LocalizedText& operator=(const char* text_) {
				this->text = text_;
				this->locale = "en";
				return *this;
			}

			bool operator==(const LocalizedText& other) const
			{
				return locale == other.locale && text == other.text;
			}
			std::string toString() const { return "\"" + locale + "\":\"" + text + "\""; }

			std::string locale;
			std::string text;
		};
	}
}
#endif
