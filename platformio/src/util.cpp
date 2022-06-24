#include <Arduino.h>

#include "util.h"

/* Takes a String and capitalizes the first letter of every word.
 *
 * Ex: 
 *   input   : "severe thunderstorm warning" or "SEVERE THUNDERSTORM WARNING"
 *   becomes : "Severe Thunderstorm Warning"
 */
void toTitleCase(String *text) {
  text->setCharAt(0, toUpperCase(text->charAt(0)) );

	for (int i = 1; i < text->length(); ++i) {
		if (text->charAt(i - 1) == ' ' 
     || text->charAt(i - 1) == '-' 
     || text->charAt(i - 1) == '(') {
			text->setCharAt(i, toUpperCase(text->charAt(i)) );
		} else {
      text->setCharAt(i, toLowerCase(text->charAt(i)) );
    }
	}

	return;
}

/* Takes a String and truncates at any of these characters ,.( and trims any
 * trailing whitespace.
 *
 * Ex: 
 *   input   : "Severe Thunderstorm Warning, (Starting At 10 Pm)"
 *   becomes : "Severe Thunderstorm Warning"
 */
void truncateExtraneousInfo(String *text) {
  if (text->isEmpty()) {
    return;
  }

  int i = 1;
  int lastChar = i;
  while (i < text->length() && (text->charAt(i) != ',' 
                                || text->charAt(i) != '.' 
                                || text->charAt(i) != '(' 
                               ) )
  {
    if (text->charAt(i) != ' ') {
      lastChar = i + 1;
    }
    ++i;
  }

  *text = text->substring(0, lastChar);

	return;
}
