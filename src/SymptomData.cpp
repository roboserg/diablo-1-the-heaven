#include "stdafx.h"

Perk SymptomPerks[] = {

	{ SYMP_MALAISE, {"You feel unwell,::    hit points: -%i%%"}, "Malaise", {
		{  2, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{  4, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{  7, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 10, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 13, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
	} },

	{ SYMP_PALLOR, {"Suppressed blood flow::gives you deathly::white appearance,::    mana: -%i%%"}, "Pallor", {
		{  2, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{  5, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{  8, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 11, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 14, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
	} },

	{ SYMP_FEVER_AND_CHILLS, {"Increased temperature::and severe dehydration,::    armor class: -%i"}, "Fever and Chills", {
		{  3, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,   5 },
		{  6, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  10 },
		{  9, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  20 },
		{ 12, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  35 },
		{ 15, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  50 },
	} },

	{ SYMP_COUGHING, {"Intensive coughing,::    mana regeneration: -%i"}, "Coughing", {
		{  4, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  1 },
		{  7, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  2 },
		{ 10, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  3 },
		{ 13, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  4 },
		{ 16, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 20, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  7 },
		{ 24, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  9 },
		{ 27, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 11 },
		{ 30, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 13 },
		{ 33, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
	} },

	{ SYMP_BLOODY_SPUTUM, {"Coughing up blood,::    life regeneration: -%i"}, "Bloody Sputum", {
		{  5, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  1 },
		{  8, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  2 },
		{ 11, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  3 },
		{ 14, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  4 },
		{ 17, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 20, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  7 },
		{ 22, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  9 },
		{ 24, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 11 },
		{ 26, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 13 },
		{ 28, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 30, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 17 },
		{ 31, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 19 },
		{ 32, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 21 },
		{ 33, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 23 },
		{ 34, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },

	} },

	{ SYMP_FIBROSIS, {"Lungs fill with blood,::causes shortness of breath,::life and mana regeneration::    gets reduced by %i%%"}, "Fibrosis", {
		{ 17, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 20, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 23, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 26, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
		{ 29, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35 },
		{ 32, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 40 },
		{ 35, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 45 },
		{ 38, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 50 },
		{ 41, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 55 },
		{ 44, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 60 },
		{ 47, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 65 },
		{ 50, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 70 },
	} },

	{ SYMP_CYSTS, {"painful lumps appear::on your neck,::    damage from enemies: +%i"}, "Cysts", {
		{  3, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
		{  6, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2 },
		{  9, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 3 },
		{ 12, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 4 },
		{ 15, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 5 },
	} },

	{ SYMP_DARK_PUSTULES, {"Cysts become black::and fill with dark pus,::    damage from enemies: +%i"}, "Dark Pustules", {
		{ 16, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  2 },
		{ 19, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  4 },
		{ 22, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  6 },
		{ 25, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  8 },
		{ 28, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },
	
	{ SYMP_BUBOES, {"Lymph nodes get swolen::causing pain and suffering,::     damage from enemies: +%i"}, "Buboes", {
		{ 29, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  3 },
		{ 32, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  6 },
		{ 35, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  9 },
		{ 38, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 12 },
		{ 41, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
	} },

	{ SYMP_SEPSIS, {"Infection spreads::to inner organs::poisoning bloodstream,::    life regeneration: -%i"}, "Sepsis", {
		{ 35, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 37, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 39, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 41, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 43, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
	} },

	{ SYMP_INTERNAL_BLEEDING, {"Reduced blood clotting,::severe internal bleeding,::    life regeneration: -%i"}, "Internal Bleeding", {
		{ 44, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 45, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 46, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
		{ 47, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 40 },
		{ 48, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 50 },
	} },

	{ SYMP_NECROSIS, {"infected tissue loses::blood supply and becomes::sources of gangrene,::    life regeneration: -%i"}, "Necrosis", {
		{  50, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  10 },
		{  60, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  20 },
		{  70, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  30 },
		{  80, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  40 },
		{  90, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  50 },
		{ 100, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  60 },
		{ 110, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  70 },
		{ 120, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  80 },
		{ 130, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  90 },
		{ 140, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 100 },
		{ 150, IPCM_ALL_CLASSES, {}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 110 },
	} },

};
