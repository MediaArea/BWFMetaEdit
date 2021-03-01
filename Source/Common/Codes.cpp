// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Common/Codes.h"
#include "ZenLib/Ztring.h"
#include <utility>

using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// Countries - CSET
//***************************************************************************

const codes::value_type CCountries_Raw[] = {
    codes::value_type(1, "USA"),
    codes::value_type(2, "Canada"),
    codes::value_type(3, "Latin America"),
    codes::value_type(30, "Greece"),
    codes::value_type(31, "Netherlands"),
    codes::value_type(32, "Belgium"),
    codes::value_type(33, "France"),
    codes::value_type(34, "Spain"),
    codes::value_type(39, "Italy"),
    codes::value_type(41, "Switzerland"),
    codes::value_type(43, "Austria"),
    codes::value_type(44, "United Kingdom"),
    codes::value_type(45, "Denmark"),
    codes::value_type(46, "Sweden"),
    codes::value_type(47, "Norway"),
    codes::value_type(49, "Germany"),
    codes::value_type(52, "Mexico"),
    codes::value_type(55, "Brazil"),
    codes::value_type(61, "Australia"),
    codes::value_type(64, "New Zealand"),
    codes::value_type(81, "Japan"),
    codes::value_type(82, "Korea"),
    codes::value_type(86, "China"),
    codes::value_type(88, "Taiwan"),
    codes::value_type(90, "Turkey"),
    codes::value_type(351, "Portugal"),
    codes::value_type(352, "Luxembourg"),
    codes::value_type(354, "Iceland"),
    codes::value_type(358, "Finland")
};

codes CCountries(CCountries_Raw, CCountries_Raw+(sizeof CCountries_Raw / sizeof CCountries_Raw[0]));

//***************************************************************************
// Countries - ISO
//***************************************************************************

const codes::value_type ISOCountries_Raw[] = {
    codes::value_type(0x4146, "Afghanistan"),
    codes::value_type(0x414C, "Albania"),
    codes::value_type(0x445A, "Algeria"),
    codes::value_type(0x4153, "American Samoa"),
    codes::value_type(0x4144, "Andorra"),
    codes::value_type(0x414F, "Angola"),
    codes::value_type(0x4149, "Anguilla"),
    codes::value_type(0x4151, "Antarctica"),
    codes::value_type(0x4147, "Antigua and Barbuda"),
    codes::value_type(0x4152, "Argentina"),
    codes::value_type(0x414D, "Armenia"),
    codes::value_type(0x4157, "Aruba"),
    codes::value_type(0x4155, "Australia"),
    codes::value_type(0x4154, "Austria"),
    codes::value_type(0x415A, "Azerbaijan"),
    codes::value_type(0x4253, "Bahamas"),
    codes::value_type(0x4248, "Bahrain"),
    codes::value_type(0x4244, "Bangladesh"),
    codes::value_type(0x4242, "Barbados"),
    codes::value_type(0x4259, "Belarus"),
    codes::value_type(0x4245, "Belgium"),
    codes::value_type(0x425A, "Belize"),
    codes::value_type(0x424A, "Benin"),
    codes::value_type(0x424D, "Bermuda"),
    codes::value_type(0x4254, "Bhutan"),
    codes::value_type(0x424F, "Bolivia, Plurinational State of"),
    codes::value_type(0x424F, "Bolivia"),
    codes::value_type(0x4241, "Bosnia and Herzegovina"),
    codes::value_type(0x4257, "Botswana"),
    codes::value_type(0x4256, "Bouvet Island"),
    codes::value_type(0x4252, "Brazil"),
    codes::value_type(0x494F, "British Indian Ocean Territory"),
    codes::value_type(0x424E, "Brunei Darussalam"),
    codes::value_type(0x424E, "Brunei"),
    codes::value_type(0x4247, "Bulgaria"),
    codes::value_type(0x4246, "Burkina Faso"),
    codes::value_type(0x4249, "Burundi"),
    codes::value_type(0x4B48, "Cambodia"),
    codes::value_type(0x434D, "Cameroon"),
    codes::value_type(0x4341, "Canada"),
    codes::value_type(0x4356, "Cape Verde"),
    codes::value_type(0x4B59, "Cayman Islands"),
    codes::value_type(0x4346, "Central African Republic"),
    codes::value_type(0x5444, "Chad"),
    codes::value_type(0x434C, "Chile"),
    codes::value_type(0x434E, "China"),
    codes::value_type(0x4358, "Christmas Island"),
    codes::value_type(0x4343, "Cocos (Keeling) Islands"),
    codes::value_type(0x434F, "Colombia"),
    codes::value_type(0x4B4D, "Comoros"),
    codes::value_type(0x4347, "Congo"),
    codes::value_type(0x4344, "Congo, the Democratic Republic of the"),
    codes::value_type(0x434B, "Cook Islands"),
    codes::value_type(0x4352, "Costa Rica"),
    codes::value_type(0x4349, "C?te d'Ivoire"),
    codes::value_type(0x4349, "Ivory Coast"),
    codes::value_type(0x4852, "Croatia"),
    codes::value_type(0x4355, "Cuba"),
    codes::value_type(0x4359, "Cyprus"),
    codes::value_type(0x435A, "Czech Republic"),
    codes::value_type(0x444B, "Denmark"),
    codes::value_type(0x444A, "Djibouti"),
    codes::value_type(0x444D, "Dominica"),
    codes::value_type(0x444F, "Dominican Republic"),
    codes::value_type(0x4543, "Ecuador"),
    codes::value_type(0x4547, "Egypt"),
    codes::value_type(0x5356, "El Salvador"),
    codes::value_type(0x4751, "Equatorial Guinea"),
    codes::value_type(0x4552, "Eritrea"),
    codes::value_type(0x4545, "Estonia"),
    codes::value_type(0x4554, "Ethiopia"),
    codes::value_type(0x464B, "Falkland Islands (Malvinas)"),
    codes::value_type(0x464F, "Faroe Islands"),
    codes::value_type(0x464A, "Fiji"),
    codes::value_type(0x4649, "Finland"),
    codes::value_type(0x4652, "France"),
    codes::value_type(0x4746, "French Guiana"),
    codes::value_type(0x5046, "French Polynesia"),
    codes::value_type(0x5446, "French Southern Territories"),
    codes::value_type(0x4741, "Gabon"),
    codes::value_type(0x474D, "Gambia"),
    codes::value_type(0x4745, "Georgia"),
    codes::value_type(0x4445, "Germany"),
    codes::value_type(0x4748, "Ghana"),
    codes::value_type(0x4749, "Gibraltar"),
    codes::value_type(0x4752, "Greece"),
    codes::value_type(0x474C, "Greenland"),
    codes::value_type(0x4744, "Grenada"),
    codes::value_type(0x4750, "Guadeloupe"),
    codes::value_type(0x4755, "Guam"),
    codes::value_type(0x4754, "Guatemala"),
    codes::value_type(0x4747, "Guernsey"),
    codes::value_type(0x474E, "Guinea"),
    codes::value_type(0x4757, "Guinea-Bissau"),
    codes::value_type(0x4759, "Guyana"),
    codes::value_type(0x4854, "Haiti"),
    codes::value_type(0x484D, "Heard Island and McDonald Islands"),
    codes::value_type(0x5641, "Holy See (Vatican City State)"),
    codes::value_type(0x484E, "Honduras"),
    codes::value_type(0x484B, "Hong Kong"),
    codes::value_type(0x4855, "Hungary"),
    codes::value_type(0x4953, "Iceland"),
    codes::value_type(0x494E, "India"),
    codes::value_type(0x4944, "Indonesia"),
    codes::value_type(0x4952, "Iran, Islamic Republic of"),
    codes::value_type(0x4951, "Iraq"),
    codes::value_type(0x4945, "Ireland"),
    codes::value_type(0x494D, "Isle of Man"),
    codes::value_type(0x494C, "Israel"),
    codes::value_type(0x4954, "Italy"),
    codes::value_type(0x4A4D, "Jamaica"),
    codes::value_type(0x4A50, "Japan"),
    codes::value_type(0x4A45, "Jersey"),
    codes::value_type(0x4A4F, "Jordan"),
    codes::value_type(0x4B5A, "Kazakhstan"),
    codes::value_type(0x4B45, "Kenya"),
    codes::value_type(0x4B49, "Kiribati"),
    codes::value_type(0x4B50, "Korea, Democratic People's Republic of"),
    codes::value_type(0x4B52, "Korea, Republic of"),
    codes::value_type(0x4B52, "South Korea"),
    codes::value_type(0x4B57, "Kuwait"),
    codes::value_type(0x4B47, "Kyrgyzstan"),
    codes::value_type(0x4C41, "Lao People's Democratic Republic"),
    codes::value_type(0x4C56, "Latvia"),
    codes::value_type(0x4C42, "Lebanon"),
    codes::value_type(0x4C53, "Lesotho"),
    codes::value_type(0x4C52, "Liberia"),
    codes::value_type(0x4C59, "Libyan Arab Jamahiriya"),
    codes::value_type(0x4C59, "Libya"),
    codes::value_type(0x4C49, "Liechtenstein"),
    codes::value_type(0x4C54, "Lithuania"),
    codes::value_type(0x4C55, "Luxembourg"),
    codes::value_type(0x4D4F, "Macao"),
    codes::value_type(0x4D4B, "Macedonia, the former Yugoslav Republic of"),
    codes::value_type(0x4D47, "Madagascar"),
    codes::value_type(0x4D57, "Malawi"),
    codes::value_type(0x4D59, "Malaysia"),
    codes::value_type(0x4D56, "Maldives"),
    codes::value_type(0x4D4C, "Mali"),
    codes::value_type(0x4D54, "Malta"),
    codes::value_type(0x4D48, "Marshall Islands"),
    codes::value_type(0x4D51, "Martinique"),
    codes::value_type(0x4D52, "Mauritania"),
    codes::value_type(0x4D55, "Mauritius"),
    codes::value_type(0x5954, "Mayotte"),
    codes::value_type(0x4D58, "Mexico"),
    codes::value_type(0x464D, "Micronesia, Federated States of"),
    codes::value_type(0x4D44, "Moldova, Republic of"),
    codes::value_type(0x4D43, "Monaco"),
    codes::value_type(0x4D4E, "Mongolia"),
    codes::value_type(0x4D45, "Montenegro"),
    codes::value_type(0x4D53, "Montserrat"),
    codes::value_type(0x4D41, "Morocco"),
    codes::value_type(0x4D5A, "Mozambique"),
    codes::value_type(0x4D4D, "Myanmar"),
    codes::value_type(0x4D4D, "Burma"),
    codes::value_type(0x4E41, "Namibia"),
    codes::value_type(0x4E52, "Nauru"),
    codes::value_type(0x4E50, "Nepal"),
    codes::value_type(0x4E4C, "Netherlands"),
    codes::value_type(0x414E, "Netherlands Antilles"),
    codes::value_type(0x4E43, "New Caledonia"),
    codes::value_type(0x4E5A, "New Zealand"),
    codes::value_type(0x4E49, "Nicaragua"),
    codes::value_type(0x4E45, "Niger"),
    codes::value_type(0x4E47, "Nigeria"),
    codes::value_type(0x4E55, "Niue"),
    codes::value_type(0x4E46, "Norfolk Island"),
    codes::value_type(0x4D50, "Northern Mariana Islands"),
    codes::value_type(0x4E4F, "Norway"),
    codes::value_type(0x4F4D, "Oman"),
    codes::value_type(0x504B, "Pakistan"),
    codes::value_type(0x5057, "Palau"),
    codes::value_type(0x5053, "Palestinian Territory, Occupied"),
    codes::value_type(0x5041, "Panama"),
    codes::value_type(0x5047, "Papua New Guinea"),
    codes::value_type(0x5059, "Paraguay"),
    codes::value_type(0x5045, "Peru"),
    codes::value_type(0x5048, "Philippines"),
    codes::value_type(0x504E, "Pitcairn"),
    codes::value_type(0x504C, "Poland"),
    codes::value_type(0x5054, "Portugal"),
    codes::value_type(0x5052, "Puerto Rico"),
    codes::value_type(0x5141, "Qatar"),
    codes::value_type(0x5245, "R?union"),
    codes::value_type(0x524F, "Romania"),
    codes::value_type(0x5255, "Russian Federation"),
    codes::value_type(0x5255, "Russia"),
    codes::value_type(0x5257, "Rwanda"),
    codes::value_type(0x5348, "Saint Helena, Ascension and Tristan da Cunha"),
    codes::value_type(0x4B4E, "Saint Kitts and Nevis"),
    codes::value_type(0x4C43, "Saint Lucia"),
    codes::value_type(0x504D, "Saint Pierre and Miquelon"),
    codes::value_type(0x5643, "Saint Vincent and the Grenadines"),
    codes::value_type(0x5643, "Saint Vincent & the Grenadines"),
    codes::value_type(0x5643, "St. Vincent and the Grenadines"),
    codes::value_type(0x5753, "Samoa"),
    codes::value_type(0x534D, "San Marino"),
    codes::value_type(0x5354, "Sao Tome and Principe"),
    codes::value_type(0x5341, "Saudi Arabia"),
    codes::value_type(0x534E, "Senegal"),
    codes::value_type(0x5253, "Serbia"),
    codes::value_type(0x5343, "Seychelles"),
    codes::value_type(0x534C, "Sierra Leone"),
    codes::value_type(0x5347, "Singapore"),
    codes::value_type(0x534B, "Slovakia"),
    codes::value_type(0x5349, "Slovenia"),
    codes::value_type(0x5342, "Solomon Islands"),
    codes::value_type(0x534F, "Somalia"),
    codes::value_type(0x5A41, "South Africa"),
    codes::value_type(0x4753, "South Georgia and the South Sandwich Islands"),
    codes::value_type(0x5353, "South Sudan"),
    codes::value_type(0x4553, "Spain"),
    codes::value_type(0x4C4B, "Sri Lanka"),
    codes::value_type(0x5344, "Sudan"),
    codes::value_type(0x5352, "Suriname"),
    codes::value_type(0x534A, "Svalbard and Jan Mayen"),
    codes::value_type(0x535A, "Swaziland"),
    codes::value_type(0x5345, "Sweden"),
    codes::value_type(0x4348, "Switzerland"),
    codes::value_type(0x5359, "Syrian Arab Republic"),
    codes::value_type(0x5457, "Taiwan, Province of China"),
    codes::value_type(0x5457, "Taiwan"),
    codes::value_type(0x544A, "Tajikistan"),
    codes::value_type(0x545A, "Tanzania, United Republic of"),
    codes::value_type(0x5448, "Thailand"),
    codes::value_type(0x544C, "Timor-Leste"),
    codes::value_type(0x5447, "Togo"),
    codes::value_type(0x544B, "Tokelau"),
    codes::value_type(0x544F, "Tonga"),
    codes::value_type(0x5454, "Trinidad and Tobago"),
    codes::value_type(0x544E, "Tunisia"),
    codes::value_type(0x5452, "Turkey"),
    codes::value_type(0x544D, "Turkmenistan"),
    codes::value_type(0x5443, "Turks and Caicos Islands"),
    codes::value_type(0x5456, "Tuvalu"),
    codes::value_type(0x5547, "Uganda"),
    codes::value_type(0x5541, "Ukraine"),
    codes::value_type(0x4145, "United Arab Emirates"),
    codes::value_type(0x4742, "United Kingdom"),
    codes::value_type(0x5553, "United States"),
    codes::value_type(0x554D, "United States Minor Outlying Islands"),
    codes::value_type(0x5559, "Uruguay"),
    codes::value_type(0x555A, "Uzbekistan"),
    codes::value_type(0x5655, "Vanuatu"),
    codes::value_type(0x5645, "Venezuela, Bolivarian Republic of"),
    codes::value_type(0x5645, "Venezuela"),
    codes::value_type(0x564E, "Viet Nam"),
    codes::value_type(0x564E, "Vietnam"),
    codes::value_type(0x5647, "Virgin Islands, British"),
    codes::value_type(0x5649, "Virgin Islands, U.S."),
    codes::value_type(0x5746, "Wallis and Futuna"),
    codes::value_type(0x4548, "Western Sahara"),
    codes::value_type(0x5945, "Yemen"),
    codes::value_type(0x5A4D, "Zambia"),
    codes::value_type(0x5A57, "Zimbabwe"),
};

codes ISOCountries(ISOCountries_Raw, ISOCountries_Raw+(sizeof ISOCountries_Raw / sizeof ISOCountries_Raw[0]));

//***************************************************************************
// Languages - CSET
//***************************************************************************

const codes::value_type CLanguages_Raw[] = {
    codes::value_type(1, "Arabic"),
    codes::value_type(2, "Bulgarian"),
    codes::value_type(3, "Catalan"),
    codes::value_type(4, "Chinese"),
    codes::value_type(5, "Czech"),
    codes::value_type(6, "Danish"),
    codes::value_type(7, "German"),
    codes::value_type(8, "Greek"),
    codes::value_type(9, "English"),
    codes::value_type(10, "Spanish"),
    codes::value_type(11, "Finnish"),
    codes::value_type(12, "French"),
    codes::value_type(13, "Hebrew"),
    codes::value_type(14, "Hungarian"),
    codes::value_type(15, "Icelandic"),
    codes::value_type(16, "Italian"),
    codes::value_type(17, "Japanese"),
    codes::value_type(18, "Korean"),
    codes::value_type(19, "Dutch"),
    codes::value_type(20, "Norwegian"),
    codes::value_type(21, "Polish"),
    codes::value_type(22, "Portuguese"),
    codes::value_type(23, "Rhaeto-Romanic"),
    codes::value_type(24, "Romanian"),
    codes::value_type(25, "Russian"),
    codes::value_type(26, "Serbo-Croatian"),
    codes::value_type(27, "Slovak"),
    codes::value_type(28, "Albanian"),
    codes::value_type(29, "Swedish"),
    codes::value_type(30, "Thai"),
    codes::value_type(31, "Turkish"),
    codes::value_type(32, "Urdu"),
    codes::value_type(33, "Bahasa")
};

codes CLanguages(CLanguages_Raw, CLanguages_Raw+(sizeof CLanguages_Raw / sizeof CLanguages_Raw[0]));

//***************************************************************************
// Dialects - CSET
//***************************************************************************

const codes::value_type CDialects_Raw[] = {
    codes::value_type(11, "Arabic"),
    codes::value_type(21, "Bulgarian"),
    codes::value_type(31, "Catalan"),
    codes::value_type(41, "Traditional"),
    codes::value_type(42, "Simplified"),
    codes::value_type(51, "Czech"),
    codes::value_type(61, "Danish"),
    codes::value_type(71, "German"),
    codes::value_type(72, "Swiss"),
    codes::value_type(81, "Greek"),
    codes::value_type(91, "US"),
    codes::value_type(92, "UK"),
    codes::value_type(101, "Spanish"),
    codes::value_type(102, "Mexican"),
    codes::value_type(111, "Finnish"),
    codes::value_type(121, "French"),
    codes::value_type(122, "Belgian"),
    codes::value_type(123, "Canadian"),
    codes::value_type(124, "Swiss"),
    codes::value_type(131, "Hebrew"),
    codes::value_type(141, "Hungarian"),
    codes::value_type(151, "Icelandic"),
    codes::value_type(161, "Italian"),
    codes::value_type(162, "Swiss"),
    codes::value_type(171, "Japanese"),
    codes::value_type(181, "Korean"),
    codes::value_type(191, "Dutch"),
    codes::value_type(192, "Belgian"),
    codes::value_type(201, "Bokmal"),
    codes::value_type(202, "Nynorsk"),
    codes::value_type(211, "Polish"),
    codes::value_type(221, "Bresilian"),
    codes::value_type(222, "Portuguese"),
    codes::value_type(231, "Rhaeto-Romanic"),
    codes::value_type(241, "Romanian"),
    codes::value_type(251, "Russian"),
    codes::value_type(261, "Latin"),
    codes::value_type(262, "Cyrilic"),
    codes::value_type(271, "Slovak"),
    codes::value_type(281, "Albanian"),
    codes::value_type(291, "Swedish"),
    codes::value_type(301, "Thai"),
    codes::value_type(311, "Turkish"),
    codes::value_type(321, "Urdu"),
    codes::value_type(331, "Bahasa")
};

codes CDialects(CDialects_Raw, CDialects_Raw+(sizeof CDialects_Raw / sizeof CDialects_Raw[0]));

//***************************************************************************
// Languages - ISO
//***************************************************************************

const codes::value_type ISOLanguages_Raw[] = {
    codes::value_type(0x6161, "Afar"),
    codes::value_type(0x6162, "Abkhazian"),
    codes::value_type(0x6165, "Avestan"),
    codes::value_type(0x6166, "Afrikaans"),
    codes::value_type(0x616B, "Akan"),
    codes::value_type(0x616D, "Amharic"),
    codes::value_type(0x616E, "Aragonese"),
    codes::value_type(0x6172, "Arabic"),
    codes::value_type(0x6173, "Assamese"),
    codes::value_type(0x6176, "Avaric"),
    codes::value_type(0x6179, "Aymara"),
    codes::value_type(0x617A, "Azerbaijani"),
    codes::value_type(0x6261, "Bashkir"),
    codes::value_type(0x6265, "Belarusian"),
    codes::value_type(0x6267, "Bulgarian"),
    codes::value_type(0x6268, "Bihari languages"),
    codes::value_type(0x6269, "Bislama"),
    codes::value_type(0x626D, "Bambara"),
    codes::value_type(0x626E, "Bengali"),
    codes::value_type(0x626F, "Tibetan"),
    codes::value_type(0x6272, "Breton"),
    codes::value_type(0x6273, "Bosnian"),
    codes::value_type(0x6361, "Catalan"),
    codes::value_type(0x6365, "Chechen"),
    codes::value_type(0x6368, "Chamorro"),
    codes::value_type(0x636F, "Corsican"),
    codes::value_type(0x6372, "Cree"),
    codes::value_type(0x6373, "Czech"),
    codes::value_type(0x6375, "Church Slavic"),
    codes::value_type(0x6376, "Chuvash"),
    codes::value_type(0x6379, "Welsh"),
    codes::value_type(0x6461, "Danish"),
    codes::value_type(0x6465, "German"),
    codes::value_type(0x6476, "Divehi"),
    codes::value_type(0x647A, "Dzongkha"),
    codes::value_type(0x6565, "Ewe"),
    codes::value_type(0x656C, "Greek"),
    codes::value_type(0x656E, "English"),
    codes::value_type(0x656F, "Esperanto"),
    codes::value_type(0x6573, "Spanish"),
    codes::value_type(0x6574, "Estonian"),
    codes::value_type(0x6575, "Basque"),
    codes::value_type(0x6661, "Persian"),
    codes::value_type(0x6666, "Fulah"),
    codes::value_type(0x6669, "Finnish"),
    codes::value_type(0x666A, "Fijian"),
    codes::value_type(0x666F, "Faroese"),
    codes::value_type(0x6672, "French"),
    codes::value_type(0x6679, "Western Frisian"),
    codes::value_type(0x6761, "Irish"),
    codes::value_type(0x6764, "Gaelic"),
    codes::value_type(0x676C, "Galician"),
    codes::value_type(0x676E, "Guarani"),
    codes::value_type(0x6775, "Gujarati"),
    codes::value_type(0x6776, "Manx"),
    codes::value_type(0x6861, "Hausa"),
    codes::value_type(0x6865, "Hebrew"),
    codes::value_type(0x6869, "Hindi"),
    codes::value_type(0x686F, "Hiri Motu"),
    codes::value_type(0x6872, "Croatian"),
    codes::value_type(0x6874, "Haitian"),
    codes::value_type(0x6875, "Hungarian"),
    codes::value_type(0x6879, "Armenian"),
    codes::value_type(0x687A, "Herero"),
    codes::value_type(0x6961, "Interlingua (International Auxiliary Language Association)"),
    codes::value_type(0x6964, "Indonesian"),
    codes::value_type(0x6965, "Interlingue"),
    codes::value_type(0x6967, "Igbo"),
    codes::value_type(0x6969, "Sichuan Yi"),
    codes::value_type(0x696B, "Inupiaq"),
    codes::value_type(0x696F, "Ido"),
    codes::value_type(0x6973, "Icelandic"),
    codes::value_type(0x6974, "Italian"),
    codes::value_type(0x6975, "Inuktitut"),
    codes::value_type(0x6A61, "Japanese"),
    codes::value_type(0x6A76, "Javanese"),
    codes::value_type(0x6B61, "Georgian"),
    codes::value_type(0x6B67, "Kongo"),
    codes::value_type(0x6B69, "Kikuyu"),
    codes::value_type(0x6B6A, "Kuanyama"),
    codes::value_type(0x6B6B, "Kazakh"),
    codes::value_type(0x6B6C, "Kalaallisut"),
    codes::value_type(0x6B6D, "Central Khmer"),
    codes::value_type(0x6B6E, "Kannada"),
    codes::value_type(0x6B6F, "Korean"),
    codes::value_type(0x6B72, "Kanuri"),
    codes::value_type(0x6B73, "Kashmiri"),
    codes::value_type(0x6B75, "Kurdish"),
    codes::value_type(0x6B76, "Komi"),
    codes::value_type(0x6B77, "Cornish"),
    codes::value_type(0x6B79, "Kirghiz"),
    codes::value_type(0x6C61, "Latin"),
    codes::value_type(0x6C62, "Luxembourgish"),
    codes::value_type(0x6C67, "Ganda"),
    codes::value_type(0x6C69, "Limburgan"),
    codes::value_type(0x6C6E, "Lingala"),
    codes::value_type(0x6C6F, "Lao"),
    codes::value_type(0x6C74, "Lithuanian"),
    codes::value_type(0x6C75, "Luba-Katanga"),
    codes::value_type(0x6C76, "Latvian"),
    codes::value_type(0x6D67, "Malagasy"),
    codes::value_type(0x6D68, "Marshallese"),
    codes::value_type(0x6D69, "Maori"),
    codes::value_type(0x6D6B, "Macedonian"),
    codes::value_type(0x6D6C, "Malayalam"),
    codes::value_type(0x6D6E, "Mongolian"),
    codes::value_type(0x6D72, "Marathi"),
    codes::value_type(0x6D73, "Malay"),
    codes::value_type(0x6D74, "Maltese"),
    codes::value_type(0x6D79, "Burmese"),
    codes::value_type(0x6E61, "Nauru"),
    codes::value_type(0x6E62, "Norwegian Bokmål"),
    codes::value_type(0x6E64, "North Ndebele"),
    codes::value_type(0x6E65, "Nepali"),
    codes::value_type(0x6E67, "Ndonga"),
    codes::value_type(0x6E6C, "Dutch"),
    codes::value_type(0x6E6E, "Norwegian Nynorsk"),
    codes::value_type(0x6E6F, "Norwegian"),
    codes::value_type(0x6E72, "South Ndebele"),
    codes::value_type(0x6E76, "Navajo"),
    codes::value_type(0x6E79, "Chichewa"),
    codes::value_type(0x6F63, "Occitan"),
    codes::value_type(0x6F6A, "Ojibwa"),
    codes::value_type(0x6F6D, "Oromo"),
    codes::value_type(0x6F72, "Oriya"),
    codes::value_type(0x6F73, "Ossetian"),
    codes::value_type(0x7061, "Panjabi"),
    codes::value_type(0x7069, "Pali"),
    codes::value_type(0x706C, "Polish"),
    codes::value_type(0x7073, "Pushto"),
    codes::value_type(0x7074, "Portuguese"),
    codes::value_type(0x7175, "Quechua"),
    codes::value_type(0x726D, "Romansh"),
    codes::value_type(0x726E, "Rundi"),
    codes::value_type(0x726F, "Romanian"),
    codes::value_type(0x7275, "Russian"),
    codes::value_type(0x7277, "Kinyarwanda"),
    codes::value_type(0x7361, "Sanskrit"),
    codes::value_type(0x7363, "Sardinian"),
    codes::value_type(0x7364, "Sindhi"),
    codes::value_type(0x7365, "Northern Sami"),
    codes::value_type(0x7367, "Sango"),
    codes::value_type(0x7369, "Sinhala"),
    codes::value_type(0x736B, "Slovak"),
    codes::value_type(0x736C, "Slovenian"),
    codes::value_type(0x736D, "Samoan"),
    codes::value_type(0x736E, "Shona"),
    codes::value_type(0x736F, "Somali"),
    codes::value_type(0x7371, "Albanian"),
    codes::value_type(0x7372, "Serbian"),
    codes::value_type(0x7373, "Swati"),
    codes::value_type(0x7374, "Southern Sotho"),
    codes::value_type(0x7375, "Sundanese"),
    codes::value_type(0x7376, "Swedish"),
    codes::value_type(0x7377, "Swahili"),
    codes::value_type(0x7461, "Tamil"),
    codes::value_type(0x7465, "Telugu"),
    codes::value_type(0x7467, "Tajik"),
    codes::value_type(0x7468, "Thai"),
    codes::value_type(0x7469, "Tigrinya"),
    codes::value_type(0x746B, "Turkmen"),
    codes::value_type(0x746C, "Tagalog"),
    codes::value_type(0x746E, "Tswana"),
    codes::value_type(0x746F, "Tonga"),
    codes::value_type(0x7472, "Turkish"),
    codes::value_type(0x7473, "Tsonga"),
    codes::value_type(0x7474, "Tatar"),
    codes::value_type(0x7477, "Twi"),
    codes::value_type(0x7479, "Tahitian"),
    codes::value_type(0x7567, "Uighur"),
    codes::value_type(0x756B, "Ukrainian"),
    codes::value_type(0x7572, "Urdu"),
    codes::value_type(0x757A, "Uzbek"),
    codes::value_type(0x7665, "Venda"),
    codes::value_type(0x7669, "Vietnamese"),
    codes::value_type(0x766F, "Volapük"),
    codes::value_type(0x7761, "Walloon"),
    codes::value_type(0x776F, "Wolof"),
    codes::value_type(0x7868, "Xhosa"),
    codes::value_type(0x7969, "Yiddish"),
    codes::value_type(0x796F, "Yoruba"),
    codes::value_type(0x7A61, "Zhuang"),
    codes::value_type(0x7A68, "Chinese"),
    codes::value_type(0x7A75, "Zulu")
};

codes ISOLanguages(ISOLanguages_Raw, ISOLanguages_Raw+(sizeof ISOLanguages_Raw / sizeof ISOLanguages_Raw[0]));

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
bool IsCSETCountry(int16u Code)
{
    if (CCountries.find(Code)!=CCountries.end())
        return true;

    return false;
}

//---------------------------------------------------------------------------
bool IsISOCountry(int16u Code)
{
    if (ISOCountries.find(Code)!=ISOCountries.end())
        return true;

    return false;
}

//---------------------------------------------------------------------------
bool IsCSETLanguage(int16u Code)
{
    if (CLanguages.find(Code)!=CLanguages.end())
        return true;

    return false;
}

//---------------------------------------------------------------------------
bool IsCSETDialect(int16u Language, int16u Code)
{
    if (IsCSETLanguage(Language) && CDialects.find(Language*10+Code)!=CDialects.end())
        return true;

    return false;
}

//---------------------------------------------------------------------------
bool IsISOLanguage(int16u Code)
{
    if (ISOLanguages.find(Code)!=ISOLanguages.end())
        return true;

    return false;
}

//---------------------------------------------------------------------------
string Country_Get(int16u Code)
{
    if (!Code)
        return "None";
    else if (CCountries.find(Code)!=CCountries.end())
        return CCountries[Code];
    else if (ISOCountries.find(Code)!=ISOCountries.end())
        return ISOCountries[Code];

    return "Unknown";
}

//---------------------------------------------------------------------------
string Language_Get(int16u Code)
{
    if (!Code)
        return "None";
    else if (CLanguages.find(Code)!=CLanguages.end())
        return CLanguages[Code];
    else if (ISOLanguages.find(Code)!=ISOLanguages.end())
        return ISOLanguages[Code];

    return "Unknown";
}

//---------------------------------------------------------------------------
string Dialect_Get(int16u Language, int16u Code)
{
    if (!Code)
        return "None";
    else if (IsCSETLanguage(Language) && CDialects.find(Language*10+Code)!=CDialects.end())
        return CDialects[Language*10+Code];

    return "Unknown";
}

//---------------------------------------------------------------------------
codes CSETCountries_Get()
{
    return CCountries;
}

//---------------------------------------------------------------------------
codes CSETLanguages_Get()
{
    return CLanguages;
}

//---------------------------------------------------------------------------
codes CSETDialects_Get()
{
    return CDialects;
}

//---------------------------------------------------------------------------
codes ISOCountries_Get()
{
    return ISOCountries;
}

//---------------------------------------------------------------------------
codes ISOLanguages_Get()
{
    return ISOLanguages;
}
