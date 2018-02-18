#ifndef owa_localizedtext_h
#define owa_localizedtext_h
#include <stdint.h>
#include <string>

namespace OWA {
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
    LocalizedText& operator=(const LocalizedText& other) {
      if (this != &other) {
        locale = other.locale;
        text = other.text;
      }
      return *this;
    }
	LocalizedText& operator=(const std::string& text) {
		this->text = text;
		this->locale = "en";
		return *this;
	}
	LocalizedText& operator=(const char* text) {
		this->text = text;
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
#endif
