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

using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// Countries - CSET
//***************************************************************************

const codes::value_type CCountries_Raw[] = {
    codes::value_type(1, codes_entries("USA", true)),
    codes::value_type(2, codes_entries("Canada", true)),
    codes::value_type(3, codes_entries("Latin America", true)),
    codes::value_type(30, codes_entries("Greece", true)),
    codes::value_type(31, codes_entries("Netherlands", true)),
    codes::value_type(32, codes_entries("Belgium", true)),
    codes::value_type(33, codes_entries("France", true)),
    codes::value_type(34, codes_entries("Spain", true)),
    codes::value_type(39, codes_entries("Italy", true)),
    codes::value_type(41, codes_entries("Switzerland", true)),
    codes::value_type(43, codes_entries("Austria", true)),
    codes::value_type(44, codes_entries("United Kingdom", true)),
    codes::value_type(45, codes_entries("Denmark", true)),
    codes::value_type(46, codes_entries("Sweden", true)),
    codes::value_type(47, codes_entries("Norway", true)),
    codes::value_type(49, codes_entries("Germany", true)),
    codes::value_type(52, codes_entries("Mexico", true)),
    codes::value_type(55, codes_entries("Brazil", true)),
    codes::value_type(61, codes_entries("Australia", true)),
    codes::value_type(64, codes_entries("New Zealand", true)),
    codes::value_type(81, codes_entries("Japan", true)),
    codes::value_type(82, codes_entries("Korea", true)),
    codes::value_type(86, codes_entries("China", true)),
    codes::value_type(88, codes_entries("Taiwan", true)),
    codes::value_type(90, codes_entries("Turkey", true)),
    codes::value_type(351, codes_entries("Portugal", true)),
    codes::value_type(352, codes_entries("Luxembourg", true)),
    codes::value_type(354, codes_entries("Iceland", true)),
    codes::value_type(358, codes_entries("Finland", true))
};

codes CCountries(CCountries_Raw, CCountries_Raw+(sizeof CCountries_Raw / sizeof CCountries_Raw[0]));

//***************************************************************************
// Countries - ISO
//***************************************************************************

const codes::value_type ISOCountries_Raw[] = {
    codes::value_type(0x4146, codes_entries("Afghanistan", true)),
    codes::value_type(0x414C, codes_entries("Albania", true)),
    codes::value_type(0x445A, codes_entries("Algeria", true)),
    codes::value_type(0x4153, codes_entries("American Samoa", true)),
    codes::value_type(0x4144, codes_entries("Andorra", true)),
    codes::value_type(0x414F, codes_entries("Angola", true)),
    codes::value_type(0x4149, codes_entries("Anguilla", true)),
    codes::value_type(0x4151, codes_entries("Antarctica", true)),
    codes::value_type(0x4147, codes_entries("Antigua and Barbuda", true)),
    codes::value_type(0x4152, codes_entries("Argentina", true)),
    codes::value_type(0x414D, codes_entries("Armenia", true)),
    codes::value_type(0x4157, codes_entries("Aruba", true)),
    codes::value_type(0x4155, codes_entries("Australia", false)),
    codes::value_type(0x4154, codes_entries("Austria", false)),
    codes::value_type(0x415A, codes_entries("Azerbaijan", true)),
    codes::value_type(0x4253, codes_entries("Bahamas", true)),
    codes::value_type(0x4248, codes_entries("Bahrain", true)),
    codes::value_type(0x4244, codes_entries("Bangladesh", true)),
    codes::value_type(0x4242, codes_entries("Barbados", true)),
    codes::value_type(0x4259, codes_entries("Belarus", true)),
    codes::value_type(0x4245, codes_entries("Belgium", false)),
    codes::value_type(0x425A, codes_entries("Belize", true)),
    codes::value_type(0x424A, codes_entries("Benin", true)),
    codes::value_type(0x424D, codes_entries("Bermuda", true)),
    codes::value_type(0x4254, codes_entries("Bhutan", true)),
    codes::value_type(0x424F, codes_entries("Bolivia, Plurinational State of", true)),
    codes::value_type(0x424F, codes_entries("Bolivia", true)),
    codes::value_type(0x4241, codes_entries("Bosnia and Herzegovina", true)),
    codes::value_type(0x4257, codes_entries("Botswana", true)),
    codes::value_type(0x4256, codes_entries("Bouvet Island", true)),
    codes::value_type(0x4252, codes_entries("Brazil", false)),
    codes::value_type(0x494F, codes_entries("British Indian Ocean Territory", true)),
    codes::value_type(0x424E, codes_entries("Brunei Darussalam", true)),
    codes::value_type(0x424E, codes_entries("Brunei", true)),
    codes::value_type(0x4247, codes_entries("Bulgaria", true)),
    codes::value_type(0x4246, codes_entries("Burkina Faso", true)),
    codes::value_type(0x4249, codes_entries("Burundi", true)),
    codes::value_type(0x4B48, codes_entries("Cambodia", true)),
    codes::value_type(0x434D, codes_entries("Cameroon", true)),
    codes::value_type(0x4341, codes_entries("Canada", false)),
    codes::value_type(0x4356, codes_entries("Cape Verde", true)),
    codes::value_type(0x4B59, codes_entries("Cayman Islands", true)),
    codes::value_type(0x4346, codes_entries("Central African Republic", true)),
    codes::value_type(0x5444, codes_entries("Chad", true)),
    codes::value_type(0x434C, codes_entries("Chile", true)),
    codes::value_type(0x434E, codes_entries("China", false)),
    codes::value_type(0x4358, codes_entries("Christmas Island", true)),
    codes::value_type(0x4343, codes_entries("Cocos (Keeling) Islands", true)),
    codes::value_type(0x434F, codes_entries("Colombia", true)),
    codes::value_type(0x4B4D, codes_entries("Comoros", true)),
    codes::value_type(0x4347, codes_entries("Congo", true)),
    codes::value_type(0x4344, codes_entries("Congo, the Democratic Republic of the", true)),
    codes::value_type(0x434B, codes_entries("Cook Islands", true)),
    codes::value_type(0x4352, codes_entries("Costa Rica", true)),
    codes::value_type(0x4349, codes_entries("Cote d'Ivoire", true)),
    codes::value_type(0x4349, codes_entries("Ivory Coast", true)),
    codes::value_type(0x4852, codes_entries("Croatia", true)),
    codes::value_type(0x4355, codes_entries("Cuba", true)),
    codes::value_type(0x4359, codes_entries("Cyprus", true)),
    codes::value_type(0x435A, codes_entries("Czech Republic", true)),
    codes::value_type(0x444B, codes_entries("Denmark", false)),
    codes::value_type(0x444A, codes_entries("Djibouti", true)),
    codes::value_type(0x444D, codes_entries("Dominica", true)),
    codes::value_type(0x444F, codes_entries("Dominican Republic", true)),
    codes::value_type(0x4543, codes_entries("Ecuador", true)),
    codes::value_type(0x4547, codes_entries("Egypt", true)),
    codes::value_type(0x5356, codes_entries("El Salvador", true)),
    codes::value_type(0x4751, codes_entries("Equatorial Guinea", true)),
    codes::value_type(0x4552, codes_entries("Eritrea", true)),
    codes::value_type(0x4545, codes_entries("Estonia", true)),
    codes::value_type(0x4554, codes_entries("Ethiopia", true)),
    codes::value_type(0x464B, codes_entries("Falkland Islands (Malvinas)", true)),
    codes::value_type(0x464F, codes_entries("Faroe Islands", true)),
    codes::value_type(0x464A, codes_entries("Fiji", true)),
    codes::value_type(0x4649, codes_entries("Finland", false)),
    codes::value_type(0x4652, codes_entries("France", false)),
    codes::value_type(0x4746, codes_entries("French Guiana", true)),
    codes::value_type(0x5046, codes_entries("French Polynesia", true)),
    codes::value_type(0x5446, codes_entries("French Southern Territories", true)),
    codes::value_type(0x4741, codes_entries("Gabon", true)),
    codes::value_type(0x474D, codes_entries("Gambia", true)),
    codes::value_type(0x4745, codes_entries("Georgia", true)),
    codes::value_type(0x4445, codes_entries("Germany", false)),
    codes::value_type(0x4748, codes_entries("Ghana", true)),
    codes::value_type(0x4749, codes_entries("Gibraltar", true)),
    codes::value_type(0x4752, codes_entries("Greece", false)),
    codes::value_type(0x474C, codes_entries("Greenland", true)),
    codes::value_type(0x4744, codes_entries("Grenada", true)),
    codes::value_type(0x4750, codes_entries("Guadeloupe", true)),
    codes::value_type(0x4755, codes_entries("Guam", true)),
    codes::value_type(0x4754, codes_entries("Guatemala", true)),
    codes::value_type(0x4747, codes_entries("Guernsey", true)),
    codes::value_type(0x474E, codes_entries("Guinea", true)),
    codes::value_type(0x4757, codes_entries("Guinea-Bissau", true)),
    codes::value_type(0x4759, codes_entries("Guyana", true)),
    codes::value_type(0x4854, codes_entries("Haiti", true)),
    codes::value_type(0x484D, codes_entries("Heard Island and McDonald Islands", true)),
    codes::value_type(0x5641, codes_entries("Holy See (Vatican City State)", true)),
    codes::value_type(0x484E, codes_entries("Honduras", true)),
    codes::value_type(0x484B, codes_entries("Hong Kong", true)),
    codes::value_type(0x4855, codes_entries("Hungary", true)),
    codes::value_type(0x4953, codes_entries("Iceland", false)),
    codes::value_type(0x494E, codes_entries("India", true)),
    codes::value_type(0x4944, codes_entries("Indonesia", true)),
    codes::value_type(0x4952, codes_entries("Iran, Islamic Republic of", true)),
    codes::value_type(0x4951, codes_entries("Iraq", true)),
    codes::value_type(0x4945, codes_entries("Ireland", true)),
    codes::value_type(0x494D, codes_entries("Isle of Man", true)),
    codes::value_type(0x494C, codes_entries("Israel", true)),
    codes::value_type(0x4954, codes_entries("Italy", false)),
    codes::value_type(0x4A4D, codes_entries("Jamaica", true)),
    codes::value_type(0x4A50, codes_entries("Japan", false)),
    codes::value_type(0x4A45, codes_entries("Jersey", true)),
    codes::value_type(0x4A4F, codes_entries("Jordan", true)),
    codes::value_type(0x4B5A, codes_entries("Kazakhstan", true)),
    codes::value_type(0x4B45, codes_entries("Kenya", true)),
    codes::value_type(0x4B49, codes_entries("Kiribati", true)),
    codes::value_type(0x4B50, codes_entries("Korea, Democratic People's Republic of", true)),
    codes::value_type(0x4B52, codes_entries("Korea, Republic of", true)),
    codes::value_type(0x4B52, codes_entries("South Korea", false)),
    codes::value_type(0x4B57, codes_entries("Kuwait", true)),
    codes::value_type(0x4B47, codes_entries("Kyrgyzstan", true)),
    codes::value_type(0x4C41, codes_entries("Lao People's Democratic Republic", true)),
    codes::value_type(0x4C56, codes_entries("Latvia", true)),
    codes::value_type(0x4C42, codes_entries("Lebanon", true)),
    codes::value_type(0x4C53, codes_entries("Lesotho", true)),
    codes::value_type(0x4C52, codes_entries("Liberia", true)),
    codes::value_type(0x4C59, codes_entries("Libyan Arab Jamahiriya", true)),
    codes::value_type(0x4C59, codes_entries("Libya", true)),
    codes::value_type(0x4C49, codes_entries("Liechtenstein", true)),
    codes::value_type(0x4C54, codes_entries("Lithuania", true)),
    codes::value_type(0x4C55, codes_entries("Luxembourg", false)),
    codes::value_type(0x4D4F, codes_entries("Macao", true)),
    codes::value_type(0x4D4B, codes_entries("Macedonia, the former Yugoslav Republic of", true)),
    codes::value_type(0x4D47, codes_entries("Madagascar", true)),
    codes::value_type(0x4D57, codes_entries("Malawi", true)),
    codes::value_type(0x4D59, codes_entries("Malaysia", true)),
    codes::value_type(0x4D56, codes_entries("Maldives", true)),
    codes::value_type(0x4D4C, codes_entries("Mali", true)),
    codes::value_type(0x4D54, codes_entries("Malta", true)),
    codes::value_type(0x4D48, codes_entries("Marshall Islands", true)),
    codes::value_type(0x4D51, codes_entries("Martinique", true)),
    codes::value_type(0x4D52, codes_entries("Mauritania", true)),
    codes::value_type(0x4D55, codes_entries("Mauritius", true)),
    codes::value_type(0x5954, codes_entries("Mayotte", true)),
    codes::value_type(0x4D58, codes_entries("Mexico", false)),
    codes::value_type(0x464D, codes_entries("Micronesia, Federated States of", true)),
    codes::value_type(0x4D44, codes_entries("Moldova, Republic of", true)),
    codes::value_type(0x4D43, codes_entries("Monaco", true)),
    codes::value_type(0x4D4E, codes_entries("Mongolia", true)),
    codes::value_type(0x4D45, codes_entries("Montenegro", true)),
    codes::value_type(0x4D53, codes_entries("Montserrat", true)),
    codes::value_type(0x4D41, codes_entries("Morocco", true)),
    codes::value_type(0x4D5A, codes_entries("Mozambique", true)),
    codes::value_type(0x4D4D, codes_entries("Myanmar", true)),
    codes::value_type(0x4D4D, codes_entries("Burma", true)),
    codes::value_type(0x4E41, codes_entries("Namibia", true)),
    codes::value_type(0x4E52, codes_entries("Nauru", true)),
    codes::value_type(0x4E50, codes_entries("Nepal", true)),
    codes::value_type(0x4E4C, codes_entries("Netherlands", false)),
    codes::value_type(0x414E, codes_entries("Netherlands Antilles", true)),
    codes::value_type(0x4E43, codes_entries("New Caledonia", true)),
    codes::value_type(0x4E5A, codes_entries("New Zealand", false)),
    codes::value_type(0x4E49, codes_entries("Nicaragua", true)),
    codes::value_type(0x4E45, codes_entries("Niger", true)),
    codes::value_type(0x4E47, codes_entries("Nigeria", true)),
    codes::value_type(0x4E55, codes_entries("Niue", true)),
    codes::value_type(0x4E46, codes_entries("Norfolk Island", true)),
    codes::value_type(0x4D50, codes_entries("Northern Mariana Islands", true)),
    codes::value_type(0x4E4F, codes_entries("Norway", false)),
    codes::value_type(0x4F4D, codes_entries("Oman", true)),
    codes::value_type(0x504B, codes_entries("Pakistan", true)),
    codes::value_type(0x5057, codes_entries("Palau", true)),
    codes::value_type(0x5053, codes_entries("Palestinian Territory, Occupied", true)),
    codes::value_type(0x5041, codes_entries("Panama", true)),
    codes::value_type(0x5047, codes_entries("Papua New Guinea", true)),
    codes::value_type(0x5059, codes_entries("Paraguay", true)),
    codes::value_type(0x5045, codes_entries("Peru", true)),
    codes::value_type(0x5048, codes_entries("Philippines", true)),
    codes::value_type(0x504E, codes_entries("Pitcairn", true)),
    codes::value_type(0x504C, codes_entries("Poland", true)),
    codes::value_type(0x5054, codes_entries("Portugal", false)),
    codes::value_type(0x5052, codes_entries("Puerto Rico", true)),
    codes::value_type(0x5141, codes_entries("Qatar", true)),
    codes::value_type(0x5245, codes_entries("Reunion", true)),
    codes::value_type(0x524F, codes_entries("Romania", true)),
    codes::value_type(0x5255, codes_entries("Russian Federation", true)),
    codes::value_type(0x5255, codes_entries("Russia", true)),
    codes::value_type(0x5257, codes_entries("Rwanda", true)),
    codes::value_type(0x5348, codes_entries("Saint Helena, Ascension and Tristan da Cunha", true)),
    codes::value_type(0x4B4E, codes_entries("Saint Kitts and Nevis", true)),
    codes::value_type(0x4C43, codes_entries("Saint Lucia", true)),
    codes::value_type(0x504D, codes_entries("Saint Pierre and Miquelon", true)),
    codes::value_type(0x5643, codes_entries("Saint Vincent and the Grenadines", true)),
    codes::value_type(0x5643, codes_entries("Saint Vincent & the Grenadines", true)),
    codes::value_type(0x5643, codes_entries("St. Vincent and the Grenadines", true)),
    codes::value_type(0x5753, codes_entries("Samoa", true)),
    codes::value_type(0x534D, codes_entries("San Marino", true)),
    codes::value_type(0x5354, codes_entries("Sao Tome and Principe", true)),
    codes::value_type(0x5341, codes_entries("Saudi Arabia", true)),
    codes::value_type(0x534E, codes_entries("Senegal", true)),
    codes::value_type(0x5253, codes_entries("Serbia", true)),
    codes::value_type(0x5343, codes_entries("Seychelles", true)),
    codes::value_type(0x534C, codes_entries("Sierra Leone", true)),
    codes::value_type(0x5347, codes_entries("Singapore", true)),
    codes::value_type(0x534B, codes_entries("Slovakia", true)),
    codes::value_type(0x5349, codes_entries("Slovenia", true)),
    codes::value_type(0x5342, codes_entries("Solomon Islands", true)),
    codes::value_type(0x534F, codes_entries("Somalia", true)),
    codes::value_type(0x5A41, codes_entries("South Africa", true)),
    codes::value_type(0x4753, codes_entries("South Georgia and the South Sandwich Islands", true)),
    codes::value_type(0x5353, codes_entries("South Sudan", true)),
    codes::value_type(0x4553, codes_entries("Spain", false)),
    codes::value_type(0x4C4B, codes_entries("Sri Lanka", true)),
    codes::value_type(0x5344, codes_entries("Sudan", true)),
    codes::value_type(0x5352, codes_entries("Suriname", true)),
    codes::value_type(0x534A, codes_entries("Svalbard and Jan Mayen", true)),
    codes::value_type(0x535A, codes_entries("Swaziland", true)),
    codes::value_type(0x5345, codes_entries("Sweden", false)),
    codes::value_type(0x4348, codes_entries("Switzerland", false)),
    codes::value_type(0x5359, codes_entries("Syrian Arab Republic", true)),
    codes::value_type(0x5457, codes_entries("Taiwan, Province of China", true)),
    codes::value_type(0x5457, codes_entries("Taiwan", false)),
    codes::value_type(0x544A, codes_entries("Tajikistan", true)),
    codes::value_type(0x545A, codes_entries("Tanzania, United Republic of", true)),
    codes::value_type(0x5448, codes_entries("Thailand", true)),
    codes::value_type(0x544C, codes_entries("Timor-Leste", true)),
    codes::value_type(0x5447, codes_entries("Togo", true)),
    codes::value_type(0x544B, codes_entries("Tokelau", true)),
    codes::value_type(0x544F, codes_entries("Tonga", true)),
    codes::value_type(0x5454, codes_entries("Trinidad and Tobago", true)),
    codes::value_type(0x544E, codes_entries("Tunisia", true)),
    codes::value_type(0x5452, codes_entries("Turkey", false)),
    codes::value_type(0x544D, codes_entries("Turkmenistan", true)),
    codes::value_type(0x5443, codes_entries("Turks and Caicos Islands", true)),
    codes::value_type(0x5456, codes_entries("Tuvalu", true)),
    codes::value_type(0x5547, codes_entries("Uganda", true)),
    codes::value_type(0x5541, codes_entries("Ukraine", true)),
    codes::value_type(0x4145, codes_entries("United Arab Emirates", true)),
    codes::value_type(0x4742, codes_entries("United Kingdom", false)),
    codes::value_type(0x5553, codes_entries("United States", false)),
    codes::value_type(0x554D, codes_entries("United States Minor Outlying Islands", true)),
    codes::value_type(0x5559, codes_entries("Uruguay", true)),
    codes::value_type(0x555A, codes_entries("Uzbekistan", true)),
    codes::value_type(0x5655, codes_entries("Vanuatu", true)),
    codes::value_type(0x5645, codes_entries("Venezuela, Bolivarian Republic of", true)),
    codes::value_type(0x5645, codes_entries("Venezuela", true)),
    codes::value_type(0x564E, codes_entries("Viet Nam", true)),
    codes::value_type(0x564E, codes_entries("Vietnam", true)),
    codes::value_type(0x5647, codes_entries("Virgin Islands, British", true)),
    codes::value_type(0x5649, codes_entries("Virgin Islands, U.S.", true)),
    codes::value_type(0x5746, codes_entries("Wallis and Futuna", true)),
    codes::value_type(0x4548, codes_entries("Western Sahara", true)),
    codes::value_type(0x5945, codes_entries("Yemen", true)),
    codes::value_type(0x5A4D, codes_entries("Zambia", true)),
    codes::value_type(0x5A57, codes_entries("Zimbabwe", true)),
};

codes ISOCountries(ISOCountries_Raw, ISOCountries_Raw+(sizeof ISOCountries_Raw / sizeof ISOCountries_Raw[0]));

//***************************************************************************
// Languages - CSET
//***************************************************************************

const codes::value_type CLanguages_Raw[] = {
    codes::value_type(1, codes_entries("Arabic", true)),
    codes::value_type(2, codes_entries("Bulgarian", true)),
    codes::value_type(3, codes_entries("Catalan", true)),
    codes::value_type(4, codes_entries("Chinese", true)),
    codes::value_type(5, codes_entries("Czech", true)),
    codes::value_type(6, codes_entries("Danish", true)),
    codes::value_type(7, codes_entries("German", true)),
    codes::value_type(8, codes_entries("Greek", true)),
    codes::value_type(9, codes_entries("English", true)),
    codes::value_type(10, codes_entries("Spanish", true)),
    codes::value_type(11, codes_entries("Finnish", true)),
    codes::value_type(12, codes_entries("French", true)),
    codes::value_type(13, codes_entries("Hebrew", true)),
    codes::value_type(14, codes_entries("Hungarian", true)),
    codes::value_type(15, codes_entries("Icelandic", true)),
    codes::value_type(16, codes_entries("Italian", true)),
    codes::value_type(17, codes_entries("Japanese", true)),
    codes::value_type(18, codes_entries("Korean", true)),
    codes::value_type(19, codes_entries("Dutch", true)),
    codes::value_type(20, codes_entries("Norwegian", true)),
    codes::value_type(21, codes_entries("Polish", true)),
    codes::value_type(22, codes_entries("Portuguese", true)),
    codes::value_type(23, codes_entries("Rhaeto-Romanic", true)),
    codes::value_type(24, codes_entries("Romanian", true)),
    codes::value_type(25, codes_entries("Russian", true)),
    codes::value_type(26, codes_entries("Serbo-Croatian", true)),
    codes::value_type(27, codes_entries("Slovak", true)),
    codes::value_type(28, codes_entries("Albanian", true)),
    codes::value_type(29, codes_entries("Swedish", true)),
    codes::value_type(30, codes_entries("Thai", true)),
    codes::value_type(31, codes_entries("Turkish", true)),
    codes::value_type(32, codes_entries("Urdu", true)),
    codes::value_type(33, codes_entries("Bahasa", true))
};

codes CLanguages(CLanguages_Raw, CLanguages_Raw+(sizeof CLanguages_Raw / sizeof CLanguages_Raw[0]));

//***************************************************************************
// Dialects - CSET
//***************************************************************************

const codes::value_type CDialects_Raw[] = {
    codes::value_type(11, codes_entries("Arabic", true)),
    codes::value_type(21, codes_entries("Bulgarian", true)),
    codes::value_type(31, codes_entries("Catalan", true)),
    codes::value_type(41, codes_entries("Traditional", true)),
    codes::value_type(42, codes_entries("Simplified", true)),
    codes::value_type(51, codes_entries("Czech", true)),
    codes::value_type(61, codes_entries("Danish", true)),
    codes::value_type(71, codes_entries("German", true)),
    codes::value_type(72, codes_entries("Swiss", true)),
    codes::value_type(81, codes_entries("Greek", true)),
    codes::value_type(91, codes_entries("US", true)),
    codes::value_type(92, codes_entries("UK", true)),
    codes::value_type(101, codes_entries("Spanish", true)),
    codes::value_type(102, codes_entries("Mexican", true)),
    codes::value_type(111, codes_entries("Finnish", true)),
    codes::value_type(121, codes_entries("French", true)),
    codes::value_type(122, codes_entries("Belgian", true)),
    codes::value_type(123, codes_entries("Canadian", true)),
    codes::value_type(124, codes_entries("Swiss", true)),
    codes::value_type(131, codes_entries("Hebrew", true)),
    codes::value_type(141, codes_entries("Hungarian", true)),
    codes::value_type(151, codes_entries("Icelandic", true)),
    codes::value_type(161, codes_entries("Italian", true)),
    codes::value_type(162, codes_entries("Swiss", true)),
    codes::value_type(171, codes_entries("Japanese", true)),
    codes::value_type(181, codes_entries("Korean", true)),
    codes::value_type(191, codes_entries("Dutch", true)),
    codes::value_type(192, codes_entries("Belgian", true)),
    codes::value_type(201, codes_entries("Bokmal", true)),
    codes::value_type(202, codes_entries("Nynorsk", true)),
    codes::value_type(211, codes_entries("Polish", true)),
    codes::value_type(221, codes_entries("Bresilian", true)),
    codes::value_type(222, codes_entries("Portuguese", true)),
    codes::value_type(231, codes_entries("Rhaeto-Romanic", true)),
    codes::value_type(241, codes_entries("Romanian", true)),
    codes::value_type(251, codes_entries("Russian", true)),
    codes::value_type(261, codes_entries("Latin", true)),
    codes::value_type(262, codes_entries("Cyrilic", true)),
    codes::value_type(271, codes_entries("Slovak", true)),
    codes::value_type(281, codes_entries("Albanian", true)),
    codes::value_type(291, codes_entries("Swedish", true)),
    codes::value_type(301, codes_entries("Thai", true)),
    codes::value_type(311, codes_entries("Turkish", true)),
    codes::value_type(321, codes_entries("Urdu", true)),
    codes::value_type(331, codes_entries("Bahasa", true))
};

codes CDialects(CDialects_Raw, CDialects_Raw+(sizeof CDialects_Raw / sizeof CDialects_Raw[0]));

//***************************************************************************
// Languages - ISO
//***************************************************************************

const codes::value_type ISOLanguages_Raw[] = {
    codes::value_type(0x6161, codes_entries("Afar", true)),
    codes::value_type(0x6162, codes_entries("Abkhazian", true)),
    codes::value_type(0x6165, codes_entries("Avestan", true)),
    codes::value_type(0x6166, codes_entries("Afrikaans", true)),
    codes::value_type(0x616B, codes_entries("Akan", true)),
    codes::value_type(0x616D, codes_entries("Amharic", true)),
    codes::value_type(0x616E, codes_entries("Aragonese", true)),
    codes::value_type(0x6172, codes_entries("Arabic", false)),
    codes::value_type(0x6173, codes_entries("Assamese", true)),
    codes::value_type(0x6176, codes_entries("Avaric", true)),
    codes::value_type(0x6179, codes_entries("Aymara", true)),
    codes::value_type(0x617A, codes_entries("Azerbaijani", true)),
    codes::value_type(0x6261, codes_entries("Bashkir", true)),
    codes::value_type(0x6265, codes_entries("Belarusian", true)),
    codes::value_type(0x6267, codes_entries("Bulgarian", false)),
    codes::value_type(0x6268, codes_entries("Bihari languages", true)),
    codes::value_type(0x6269, codes_entries("Bislama", true)),
    codes::value_type(0x626D, codes_entries("Bambara", true)),
    codes::value_type(0x626E, codes_entries("Bengali", true)),
    codes::value_type(0x626F, codes_entries("Tibetan", true)),
    codes::value_type(0x6272, codes_entries("Breton", true)),
    codes::value_type(0x6273, codes_entries("Bosnian", true)),
    codes::value_type(0x6361, codes_entries("Catalan", false)),
    codes::value_type(0x6365, codes_entries("Chechen", true)),
    codes::value_type(0x6368, codes_entries("Chamorro", true)),
    codes::value_type(0x636F, codes_entries("Corsican", true)),
    codes::value_type(0x6372, codes_entries("Cree", true)),
    codes::value_type(0x6373, codes_entries("Czech", false)),
    codes::value_type(0x6375, codes_entries("Church Slavic", true)),
    codes::value_type(0x6376, codes_entries("Chuvash", true)),
    codes::value_type(0x6379, codes_entries("Welsh", true)),
    codes::value_type(0x6461, codes_entries("Danish", false)),
    codes::value_type(0x6465, codes_entries("German", false)),
    codes::value_type(0x6476, codes_entries("Divehi", true)),
    codes::value_type(0x647A, codes_entries("Dzongkha", true)),
    codes::value_type(0x6565, codes_entries("Ewe", true)),
    codes::value_type(0x656C, codes_entries("Greek", true)),
    codes::value_type(0x656E, codes_entries("English", true)),
    codes::value_type(0x656F, codes_entries("Esperanto", true)),
    codes::value_type(0x6573, codes_entries("Spanish", true)),
    codes::value_type(0x6574, codes_entries("Estonian", true)),
    codes::value_type(0x6575, codes_entries("Basque", true)),
    codes::value_type(0x6661, codes_entries("Persian", true)),
    codes::value_type(0x6666, codes_entries("Fulah", true)),
    codes::value_type(0x6669, codes_entries("Finnish", true)),
    codes::value_type(0x666A, codes_entries("Fijian", true)),
    codes::value_type(0x666F, codes_entries("Faroese", true)),
    codes::value_type(0x6672, codes_entries("French", false)),
    codes::value_type(0x6679, codes_entries("Western Frisian", true)),
    codes::value_type(0x6761, codes_entries("Irish", true)),
    codes::value_type(0x6764, codes_entries("Gaelic", true)),
    codes::value_type(0x676C, codes_entries("Galician", true)),
    codes::value_type(0x676E, codes_entries("Guarani", true)),
    codes::value_type(0x6775, codes_entries("Gujarati", true)),
    codes::value_type(0x6776, codes_entries("Manx", true)),
    codes::value_type(0x6861, codes_entries("Hausa", true)),
    codes::value_type(0x6865, codes_entries("Hebrew", false)),
    codes::value_type(0x6869, codes_entries("Hindi", true)),
    codes::value_type(0x686F, codes_entries("Hiri Motu", true)),
    codes::value_type(0x6872, codes_entries("Croatian", true)),
    codes::value_type(0x6874, codes_entries("Haitian", true)),
    codes::value_type(0x6875, codes_entries("Hungarian", false)),
    codes::value_type(0x6879, codes_entries("Armenian", true)),
    codes::value_type(0x687A, codes_entries("Herero", true)),
    codes::value_type(0x6961, codes_entries("Interlingua (International Auxiliary Language Association)", true)),
    codes::value_type(0x6964, codes_entries("Indonesian", true)),
    codes::value_type(0x6965, codes_entries("Interlingue", true)),
    codes::value_type(0x6967, codes_entries("Igbo", true)),
    codes::value_type(0x6969, codes_entries("Sichuan Yi", true)),
    codes::value_type(0x696B, codes_entries("Inupiaq", true)),
    codes::value_type(0x696F, codes_entries("Ido", true)),
    codes::value_type(0x6973, codes_entries("Icelandic", false)),
    codes::value_type(0x6974, codes_entries("Italian", false)),
    codes::value_type(0x6975, codes_entries("Inuktitut", true)),
    codes::value_type(0x6A61, codes_entries("Japanese", false)),
    codes::value_type(0x6A76, codes_entries("Javanese", true)),
    codes::value_type(0x6B61, codes_entries("Georgian", true)),
    codes::value_type(0x6B67, codes_entries("Kongo", true)),
    codes::value_type(0x6B69, codes_entries("Kikuyu", true)),
    codes::value_type(0x6B6A, codes_entries("Kuanyama", true)),
    codes::value_type(0x6B6B, codes_entries("Kazakh", true)),
    codes::value_type(0x6B6C, codes_entries("Kalaallisut", true)),
    codes::value_type(0x6B6D, codes_entries("Central Khmer", true)),
    codes::value_type(0x6B6E, codes_entries("Kannada", true)),
    codes::value_type(0x6B6F, codes_entries("Korean", false)),
    codes::value_type(0x6B72, codes_entries("Kanuri", true)),
    codes::value_type(0x6B73, codes_entries("Kashmiri", true)),
    codes::value_type(0x6B75, codes_entries("Kurdish", true)),
    codes::value_type(0x6B76, codes_entries("Komi", true)),
    codes::value_type(0x6B77, codes_entries("Cornish", true)),
    codes::value_type(0x6B79, codes_entries("Kirghiz", true)),
    codes::value_type(0x6C61, codes_entries("Latin", true)),
    codes::value_type(0x6C62, codes_entries("Luxembourgish", true)),
    codes::value_type(0x6C67, codes_entries("Ganda", true)),
    codes::value_type(0x6C69, codes_entries("Limburgan", true)),
    codes::value_type(0x6C6E, codes_entries("Lingala", true)),
    codes::value_type(0x6C6F, codes_entries("Lao", true)),
    codes::value_type(0x6C74, codes_entries("Lithuanian", true)),
    codes::value_type(0x6C75, codes_entries("Luba-Katanga", true)),
    codes::value_type(0x6C76, codes_entries("Latvian", true)),
    codes::value_type(0x6D67, codes_entries("Malagasy", true)),
    codes::value_type(0x6D68, codes_entries("Marshallese", true)),
    codes::value_type(0x6D69, codes_entries("Maori", true)),
    codes::value_type(0x6D6B, codes_entries("Macedonian", true)),
    codes::value_type(0x6D6C, codes_entries("Malayalam", true)),
    codes::value_type(0x6D6E, codes_entries("Mongolian", true)),
    codes::value_type(0x6D72, codes_entries("Marathi", true)),
    codes::value_type(0x6D73, codes_entries("Malay", true)),
    codes::value_type(0x6D74, codes_entries("Maltese", true)),
    codes::value_type(0x6D79, codes_entries("Burmese", true)),
    codes::value_type(0x6E61, codes_entries("Nauru", true)),
    codes::value_type(0x6E62, codes_entries("Norwegian Bokmål", false)),
    codes::value_type(0x6E64, codes_entries("North Ndebele", true)),
    codes::value_type(0x6E65, codes_entries("Nepali", true)),
    codes::value_type(0x6E67, codes_entries("Ndonga", true)),
    codes::value_type(0x6E6C, codes_entries("Dutch", false)),
    codes::value_type(0x6E6E, codes_entries("Norwegian Nynorsk", false)),
    codes::value_type(0x6E6F, codes_entries("Norwegian", false)),
    codes::value_type(0x6E72, codes_entries("South Ndebele", true)),
    codes::value_type(0x6E76, codes_entries("Navajo", true)),
    codes::value_type(0x6E79, codes_entries("Chichewa", true)),
    codes::value_type(0x6F63, codes_entries("Occitan", true)),
    codes::value_type(0x6F6A, codes_entries("Ojibwa", true)),
    codes::value_type(0x6F6D, codes_entries("Oromo", true)),
    codes::value_type(0x6F72, codes_entries("Oriya", true)),
    codes::value_type(0x6F73, codes_entries("Ossetian", true)),
    codes::value_type(0x7061, codes_entries("Panjabi", true)),
    codes::value_type(0x7069, codes_entries("Pali", true)),
    codes::value_type(0x706C, codes_entries("Polish", false)),
    codes::value_type(0x7073, codes_entries("Pushto", true)),
    codes::value_type(0x7074, codes_entries("Portuguese", false)),
    codes::value_type(0x7175, codes_entries("Quechua", true)),
    codes::value_type(0x726D, codes_entries("Romansh", true)),
    codes::value_type(0x726E, codes_entries("Rundi", true)),
    codes::value_type(0x726F, codes_entries("Romanian", false)),
    codes::value_type(0x7275, codes_entries("Russian", false)),
    codes::value_type(0x7277, codes_entries("Kinyarwanda", true)),
    codes::value_type(0x7361, codes_entries("Sanskrit", true)),
    codes::value_type(0x7363, codes_entries("Sardinian", true)),
    codes::value_type(0x7364, codes_entries("Sindhi", true)),
    codes::value_type(0x7365, codes_entries("Northern Sami", true)),
    codes::value_type(0x7367, codes_entries("Sango", true)),
    codes::value_type(0x7369, codes_entries("Sinhala", true)),
    codes::value_type(0x736B, codes_entries("Slovak", false)),
    codes::value_type(0x736C, codes_entries("Slovenian", true)),
    codes::value_type(0x736D, codes_entries("Samoan", true)),
    codes::value_type(0x736E, codes_entries("Shona", true)),
    codes::value_type(0x736F, codes_entries("Somali", true)),
    codes::value_type(0x7371, codes_entries("Albanian", false)),
    codes::value_type(0x7372, codes_entries("Serbian", true)),
    codes::value_type(0x7373, codes_entries("Swati", true)),
    codes::value_type(0x7374, codes_entries("Southern Sotho", true)),
    codes::value_type(0x7375, codes_entries("Sundanese", true)),
    codes::value_type(0x7376, codes_entries("Swedish", false)),
    codes::value_type(0x7377, codes_entries("Swahili", true)),
    codes::value_type(0x7461, codes_entries("Tamil", true)),
    codes::value_type(0x7465, codes_entries("Telugu", true)),
    codes::value_type(0x7467, codes_entries("Tajik", true)),
    codes::value_type(0x7468, codes_entries("Thai", false)),
    codes::value_type(0x7469, codes_entries("Tigrinya", true)),
    codes::value_type(0x746B, codes_entries("Turkmen", true)),
    codes::value_type(0x746C, codes_entries("Tagalog", true)),
    codes::value_type(0x746E, codes_entries("Tswana", true)),
    codes::value_type(0x746F, codes_entries("Tonga", true)),
    codes::value_type(0x7472, codes_entries("Turkish", false)),
    codes::value_type(0x7473, codes_entries("Tsonga", true)),
    codes::value_type(0x7474, codes_entries("Tatar", true)),
    codes::value_type(0x7477, codes_entries("Twi", true)),
    codes::value_type(0x7479, codes_entries("Tahitian", true)),
    codes::value_type(0x7567, codes_entries("Uighur", true)),
    codes::value_type(0x756B, codes_entries("Ukrainian", true)),
    codes::value_type(0x7572, codes_entries("Urdu", false)),
    codes::value_type(0x757A, codes_entries("Uzbek", true)),
    codes::value_type(0x7665, codes_entries("Venda", true)),
    codes::value_type(0x7669, codes_entries("Vietnamese", true)),
    codes::value_type(0x766F, codes_entries("Volapük", true)),
    codes::value_type(0x7761, codes_entries("Walloon", true)),
    codes::value_type(0x776F, codes_entries("Wolof", true)),
    codes::value_type(0x7868, codes_entries("Xhosa", true)),
    codes::value_type(0x7969, codes_entries("Yiddish", true)),
    codes::value_type(0x796F, codes_entries("Yoruba", true)),
    codes::value_type(0x7A61, codes_entries("Zhuang", true)),
    codes::value_type(0x7A68, codes_entries("Chinese", false)),
    codes::value_type(0x7A75, codes_entries("Zulu", true))
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
        return CCountries[Code].first;
    else if (ISOCountries.find(Code)!=ISOCountries.end())
        return ISOCountries[Code].first;

    return "Unknown";
}

//---------------------------------------------------------------------------
string Language_Get(int16u Code)
{
    if (!Code)
        return "None";
    else if (CLanguages.find(Code)!=CLanguages.end())
        return CLanguages[Code].first;
    else if (ISOLanguages.find(Code)!=ISOLanguages.end())
        return ISOLanguages[Code].first;

    return "Unknown";
}

//---------------------------------------------------------------------------
string Dialect_Get(int16u Language, int16u Code)
{
    if (!Code)
        return "None";
    else if (IsCSETLanguage(Language) && CDialects.find(Language*10+Code)!=CDialects.end())
        return CDialects[Language*10+Code].first;

    return "Unknown";
}

//---------------------------------------------------------------------------
const codes* CSETCountries_Get()
{
    return &CCountries;
}

//---------------------------------------------------------------------------
const codes* CSETLanguages_Get()
{
    return &CLanguages;
}

//---------------------------------------------------------------------------
const codes* CSETDialects_Get()
{
    return &CDialects;
}

//---------------------------------------------------------------------------
const codes* ISOCountries_Get()
{
    return &ISOCountries;
}

//---------------------------------------------------------------------------
const codes* ISOLanguages_Get()
{
    return &ISOLanguages;
}
