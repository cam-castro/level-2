/**
 * @brief Lequel? language identification based on trigrams
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 *
 * @cite https://towardsdatascience.com/understanding-cosine-similarity-and-its-application-fd42f585296a
*/

#include <cmath>
#include <codecvt>
#include <locale>
#include <iostream>

#include "Lequel.h"

using namespace std;

/**
 * @brief Builds a trigram profile from a given text.
 *
 * @param text Vector of lines (Text)
 * @return TrigramProfile The trigram profile
*/

TrigramProfile buildTrigramProfile(const Text &text){

    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    TrigramProfile textProfile;
    wstring unicodeTrigram;
    std::string Trigram;  

    for(auto line : text){
        if((line.length() > 0) && (line[line.length() - 1] == '\r')){
            line = line.substr(0, line.length() - 1);
        }
    }   

    for(auto sentence : text){
        wstring unicodeSentence = converter.from_bytes(sentence);
        unicodeTrigram.clear();
        Trigram.clear(); 

        for(auto character : unicodeSentence){
            if(unicodeTrigram.length() < 3){
                unicodeTrigram += character;
                if(unicodeTrigram.length() == 3){
                    Trigram = converter.to_bytes(unicodeTrigram);
                    textProfile.insert(std::make_pair(Trigram, 1)); 
                }
            }
            else if(unicodeTrigram.length() == 3){
                unicodeTrigram.erase(0, 1);
                unicodeTrigram.push_back(character);

                Trigram = converter.to_bytes(unicodeTrigram);

                if(textProfile.find(Trigram) != textProfile.end()){
                    textProfile.insert(std::make_pair(Trigram, 1));                        
                }
                else{
                    textProfile[Trigram]++;
                }
            }            
            else{
                cout<<"Oh shit"<< endl;
            }
        }
    }

    // Tip: converts UTF-8 string to wstring
    // wstring unicodeString = converter.from_bytes(textLine);

    // Tip: convert wstring to UTF-8 string
    // string trigram = converter.to_bytes(unicodeTrigram);

    return textProfile; // Fill-in result here
}

/**
 * @brief Normalizes a trigram profile.
 *
 * @param trigramProfile The trigram profile.
*/

void normalizeTrigramProfile(TrigramProfile &trigramProfile){
    for(auto &pair : trigramProfile){
        cout<<pair.first<<" "<<pair.second<<endl;
    }

    long double norm = 0;

    for(auto &pair : trigramProfile){
        norm += pair.second*pair.second;
    }
    norm = sqrt(norm);

    for(auto &pair : trigramProfile){
        pair.second = pair.second/norm;
    }
    
    return;
}

/**
 * @brief Calculates the cosine similarity between two trigram profiles
 *
 * @param textProfile The text trigram profile
 * @param languageProfile The language trigram profile
 * @return float The cosine similarity score
*/

float getCosineSimilarity(TrigramProfile &textProfile, TrigramProfile &languageProfile){
    float addition = 0;

    for(auto textKey : textProfile){
        if(languageProfile.find(textKey.first) != languageProfile.end()){
            addition += textKey.second * languageProfile[textKey.first];
        }    
    }
    return addition;
}

/**
 * @brief Identifies the language of a text.
 *
 * @param text A Text (vector of lines)
 * @param languages A list of Language objects
 * @return string The language code of the most likely language
*/

string identifyLanguage(const Text &text, LanguageProfiles &languages){
    TrigramProfile unknownLanguageTriagram = buildTrigramProfile(text);

    normalizeTrigramProfile(unknownLanguageTriagram);

    std::string unknownLanguageName;

    float cosineSimilarity, maxSimilarity = 0;

    for(auto language : languages){
        cosineSimilarity = getCosineSimilarity(unknownLanguageTriagram, language.trigramProfile);
        if(maxSimilarity == cosineSimilarity){
            unknownLanguageName = "There is more than one similarity";
        }
        else if(maxSimilarity < cosineSimilarity) {
            maxSimilarity = cosineSimilarity;
            unknownLanguageName = language.languageCode;
        } 
    }

    return unknownLanguageName; // Fill-in result here
}
