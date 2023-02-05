// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}


/* @showmobinfo 0-3
*   0: Default
*   1: Shows Size/Boss/Element/Race status
*   2: Shows Size/Boss/Element/Race status + HP%
*/
ACMD_FUNC(showmobinfo){
	nullpo_retr(-1, sd);

	int showmobinfo = 0;

	memset(atcmd_output, '\0', sizeof(atcmd_output));

	if (!message || !*message || sscanf(message, "%11d", &showmobinfo) < 1 || showmobinfo > 2) {
		sprintf(atcmd_output, "Usage: %s <0-2>", command);
		clif_displaymessage(fd, atcmd_output);
		return -1;
	}

	if (sd->sc.getSCE(SC_SHOWMOBINFO)) {
		status_change_end(&sd->bl, SC_SHOWMOBINFO);
	}

	sc_start(NULL, &sd->bl, SC_SHOWMOBINFO, 100, showmobinfo, INFINITE_TICK);
	switch(showmobinfo) {
		case 0: sprintf(atcmd_output, "You will only see the monster's name now.");
		case 1: sprintf(atcmd_output, "You will see the monster's statuses now.");
		case 2: sprintf(atcmd_output, "You will see the monster's statuses and HP percentage now.");
		default:
			break;
	}
    
    clif_displaymessage(fd, atcmd_output);

	return 0;
}


static void adv_stats_sub_resistances(map_session_data* sd, int fd){
	char temp[100];
	char output[CHAT_SIZE_MAX];
	char* output_p = output;

	bool hasElementalRes = false, hasRaceRes = false, hasClassRes = false, hasEleDefRes = false, hasSizeRes = false;

	sprintf(output, " Resistances:");
	clif_displaymessage(fd, output);

	for (int i = 0; i<ELE_MAX;i++) {
		int totalRate = sd->indexed_bonus.subele[i] + sd->indexed_bonus.subele_script[i];
		output_p = output;
		output_p += sprintf(output_p , "    %s Resistance: %d%%", melement[i], sd->indexed_bonus.subele[i] + sd->indexed_bonus.subele_script[i]);
		for (auto &s : sd->subele2)
		{
			if (s.ele == i && s.rate != 0) {
				sprintf(temp, " ");
				if (s.flag&BF_MAGIC)
					sprintf(temp, "(Magical)");
				else if (s.flag&BF_SHORT)
					sprintf(temp, "(Melee)");
				output_p += sprintf(output_p , ", %d%% %s", s.rate, temp);
				totalRate += s.rate;
			}
		}
		if (totalRate == 0)
			continue;
		if (!hasElementalRes)
			clif_displaymessage(fd, "  Resist vs Attack Element");
		hasElementalRes = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasElementalRes)
		clif_displaymessage(fd, "  No resistances against elemental attacks.");
	
	for (int i = 0; i<RC_MAX;i++) {
		int totalRate = sd->indexed_bonus.subrace[i];
		output_p = output;
		output_p += sprintf(output_p , "    %s Resistance: %d%%", mrace[i], sd->indexed_bonus.subrace[i]);
		for (auto &s : sd->subrace3)
		{
			if (s.race == i && s.rate != 0) {
				sprintf(temp, " ");
				if (s.flag&BF_NORMAL)
					sprintf(temp, "(Physical)");
				output_p += sprintf(output_p , ", %d%% %s", s.rate, temp);
				totalRate += s.rate;
			}
		}
		if (totalRate == 0)
			continue;
		if (!hasRaceRes)
			clif_displaymessage(fd, "  Resist vs Races");
		hasRaceRes = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasRaceRes)
		clif_displaymessage(fd, "  No resistances against races.");

	for (int i = 0; i<CLASS_MAX;i++) {
		int totalRate = sd->indexed_bonus.subclass[i];
		output_p = output;
		output_p += sprintf(output_p , "    %s Resistance: %d%%", mclass[i], sd->indexed_bonus.subclass[i]);
		if (totalRate == 0)
			continue;
		if (!hasClassRes)
			clif_displaymessage(fd, "  Resist vs Classes");
		hasClassRes = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasClassRes)
		clif_displaymessage(fd, "  No resistances against classes.");

	for (int i = 0; i<SZ_MAX;i++) {
		int totalRate = sd->indexed_bonus.subsize[i];
		output_p = output;
		output_p += sprintf(output_p , "    %s Resistance: %d%%", msize[i], sd->indexed_bonus.subsize[i]);
		if (totalRate == 0)
			continue;
		if (!hasSizeRes)
			clif_displaymessage(fd, "  Resist vs Sizes");
		hasSizeRes = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasSizeRes)
		clif_displaymessage(fd, "  No resistances against sizes.");

	for (int i = 0; i<ELE_MAX;i++) {
		int totalRate = sd->indexed_bonus.subdefele[i] + sd->indexed_bonus.magic_subdefele[i];
		output_p = output;
		output_p += sprintf(output_p , "    %s Mob Resistance:", melement[i]);
		if (sd->indexed_bonus.subdefele[i])
			output_p += sprintf(output_p , " %d%% (Physical) ", sd->indexed_bonus.subdefele[i]);
		if (sd->indexed_bonus.magic_subdefele[i])
			output_p += sprintf(output_p , " %d%% (Magical) ", sd->indexed_bonus.magic_subdefele[i]);
		if (totalRate == 0)
			continue;
		if (!hasEleDefRes)
			clif_displaymessage(fd, "  Resist vs Mobs w/ Elemental Defense");
		hasEleDefRes = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasEleDefRes)
		clif_displaymessage(fd, "  No resistances against elemental defense mobs.");
}

static void adv_stats_sub_skill(map_session_data* sd, int fd){
	char output[CHAT_SIZE_MAX];

	sprintf(output, " Skill Bonuses:");
	clif_displaymessage(fd, output);

	for (auto &s : sd->skillatk)
	{
		sprintf(output, "  %s Damage %s%d%%", skill_get_desc(s.id), s.val > 0 ? "+":"", s.val);
		clif_displaymessage(fd, output);
	}

	for (auto &s : sd->skillvarcast)
	{
		sprintf(output, "  %s Cast Time %s%.1fs", skill_get_desc(s.id), s.val > 0 ? "+":"", s.val/1000.);
		clif_displaymessage(fd, output);
	}

	for (auto &s : sd->skillcastrate)
	{
		sprintf(output, "  %s Cast Rate %s%d%%", skill_get_desc(s.id), -s.val > 0 ? "+":"", -s.val);
		clif_displaymessage(fd, output);
	}

	for (auto &s : sd->skillfixcast)
	{
		sprintf(output, "  %s Fixed Cast Time %s%.1fs", skill_get_desc(s.id), s.val > 0 ? "+":"", s.val/1000.);
		clif_displaymessage(fd, output);
	}

	for (auto &s : sd->skillfixcastrate)
	{
		sprintf(output, "  %s Fixed Cast Rate %s%d%%", skill_get_desc(s.id), -s.val > 0 ? "+":"", -s.val);
		clif_displaymessage(fd, output);
	}

	for (auto &s : sd->skillcooldown)
	{
		sprintf(output, "  %s Cooldown %s%.1fs", skill_get_desc(s.id), s.val > 0 ? "+":"", s.val/1000.);
		clif_displaymessage(fd, output);
	}
	
}

static void adv_stats_sub_critical(map_session_data* sd, int fd){
	char output[CHAT_SIZE_MAX];

	sprintf(output, " Critical Bonuses:\n  Critical Rate w/ Ranged Weapons: %d%%", sd->bonus.critical_rangeatk);
	clif_displaymessage(fd, output);
	sprintf(output, "  Extra Critical Damage: %d%%", sd->bonus.crit_atk_rate);
	clif_displaymessage(fd, output);
}

static void adv_stats_sub_attack(map_session_data* sd, int fd){
	char temp[100];
	char output[CHAT_SIZE_MAX];
	char* output_p = output;

	sprintf(output, " Physical Damage Bonuses:\n  Bonus Weapon/Equip ATK Modifier: %d%%", sd->bonus.atk_rate);
	clif_displaymessage(fd, output);
	sprintf(output, "  Bonus Short Range ATK Modifier: %d%%", sd->bonus.short_attack_atk_rate);
	clif_displaymessage(fd, output);
	sprintf(output, "  Bonus Long Range ATK Modifier: %d%%", sd->bonus.long_attack_atk_rate);
	clif_displaymessage(fd, output);

	bool hasElementalDmg = false, hasRace = false, hasClass = false, hasSize = false;

	for (int i = 0; i<ELE_MAX;i++) {
		int totalRate = sd->right_weapon.addele[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s Element: %d%%", melement[i], sd->right_weapon.addele[i]);
		if (totalRate == 0)
			continue;
		if (!hasElementalDmg)
			clif_displaymessage(fd, "  Physical Damage vs Target Element:");
		hasElementalDmg = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasElementalDmg)
		clif_displaymessage(fd, "  No physical damage bonus against elemental.");
	
	for (int i = 0; i<RC_MAX;i++) {
		int totalRate = sd->right_weapon.addrace[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s: %d%%", mrace[i], sd->right_weapon.addrace[i]);
		if (totalRate == 0)
			continue;
		if (!hasRace)
			clif_displaymessage(fd, "  Physical Damage vs Races:");
		hasRace = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasRace)
		clif_displaymessage(fd, "  No physical damage bonus against races.");

	for (int i = 0; i<CLASS_MAX;i++) {
		int totalRate = sd->right_weapon.addclass[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s: %d%%", mclass[i], sd->right_weapon.addclass[i]);
		if (totalRate == 0)
			continue;
		if (!hasClass)
			clif_displaymessage(fd, "  Physical Damage vs Classes:");
		hasClass = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasClass)
		clif_displaymessage(fd, "  No physical damage bonus against classes.");

	for (int i = 0; i<SZ_MAX;i++) {
		int totalRate = sd->right_weapon.addsize[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s: %d%%", msize[i], sd->right_weapon.addsize[i]);
		if (totalRate == 0)
			continue;
		if (!hasSize)
			clif_displaymessage(fd, "  Physical Damage vs Sizes:");
		hasSize = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasSize)
		clif_displaymessage(fd, "  No physical damage bonus against sizes.");


}

static void adv_stats_sub_magical(map_session_data* sd, int fd){
	char temp[100];
	char output[CHAT_SIZE_MAX];
	char* output_p = output;

	sprintf(output, " Magic Damage Bonuses:\n  Bonus MATK Modifier: %d%%", sd->matk_rate-100);
	clif_displaymessage(fd, output);

	bool hasElementalBonus=false,hasElementalDmg = false, hasRace = false, hasClass = false, hasSize = false;

	for (int i = 0; i<ELE_MAX;i++) {
		int totalRate = sd->indexed_bonus.magic_atk_ele[i];
		output_p = output;
		output_p += sprintf(output_p , "    %s Element Bonus: %d%%", melement[i], sd->indexed_bonus.magic_atk_ele[i]);
		if (totalRate == 0)
			continue;
		if (!hasElementalBonus)
			clif_displaymessage(fd, "  Elemental Magic Damage:");
		hasElementalBonus = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasElementalBonus)
		clif_displaymessage(fd, "  No elemental magic damage bonus.");

	for (int i = 0; i<ELE_MAX;i++) {
		int totalRate = sd->indexed_bonus.magic_addele[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s Element: %d%%", melement[i], sd->indexed_bonus.magic_addele[i]);
		if (totalRate == 0)
			continue;
		if (!hasElementalDmg)
			clif_displaymessage(fd, "  Magical Damage vs Target Element:");
		hasElementalDmg = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasElementalDmg)
		clif_displaymessage(fd, "  No magical damage bonus against elemental.");
	
	for (int i = 0; i<RC_MAX;i++) {
		int totalRate = sd->indexed_bonus.magic_addrace[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s: %d%%", mrace[i], sd->indexed_bonus.magic_addrace[i]);
		if (totalRate == 0)
			continue;
		if (!hasRace)
			clif_displaymessage(fd, "  Magical Damage vs Races");
		hasRace = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasRace)
		clif_displaymessage(fd, "  No magical damage bonus against races.");

	for (int i = 0; i<CLASS_MAX;i++) {
		int totalRate = sd->indexed_bonus.magic_addclass[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s: %d%%", mclass[i], sd->indexed_bonus.magic_addclass[i]);
		if (totalRate == 0)
			continue;
		if (!hasClass)
			clif_displaymessage(fd, "  Magical Damage vs Classes:");
		hasClass = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasClass)
		clif_displaymessage(fd, "  No magical damage bonus against classes.");

	for (int i = 0; i<SZ_MAX;i++) {
		int totalRate = sd->indexed_bonus.magic_addsize[i];
		output_p = output;
		output_p += sprintf(output_p , "    Damage vs %s: %d%%", msize[i], sd->indexed_bonus.magic_addsize[i]);
		if (totalRate == 0)
			continue;
		if (!hasSize)
			clif_displaymessage(fd, "  Magical Damage vs Sizes:");
		hasSize = true;
		*output_p = '\0';
		safestrncpy(temp, output, 100);
		clif_displaymessage(fd, temp);
	}

	if (!hasSize)
		clif_displaymessage(fd, "  No magical damage bonus against sizes.");

}

static void adv_stats_sub_casting(map_session_data* sd, int fd){
	char output[CHAT_SIZE_MAX];
	double castReduction = (sd->battle_status.int_ + sd->battle_status.dex * 2)/530. * 100;
	sprintf(output, " Casting Bonuses:\n  Stat-based Variable Cast Rate Reduction: %.1f%%",  castReduction > 100 ? 100 : castReduction);
	clif_displaymessage(fd, output);

	if (sd->bonus.fixcastrate != 0) {
		sprintf(output, "  Fixed Cast Rate Reduction: %d%%", sd->bonus.fixcastrate);
		clif_displaymessage(fd, output);
	}
	if (sd->bonus.add_fixcast != 0) {
		sprintf(output, "  Fixed Cast Time Modifier: %.1fs", sd->bonus.add_fixcast/1000.);
		clif_displaymessage(fd, output);
	}
	if (sd->bonus.varcastrate != 0) {
		sprintf(output, "  Variable Cast Rate Reduction: %d%%", sd->bonus.varcastrate);
		clif_displaymessage(fd, output);
	}
	if (sd->bonus.add_varcast != 0) {
		sprintf(output, "  Variable Cast Time Modifier: %.1fs", sd->bonus.add_varcast/1000.);
		clif_displaymessage(fd, output);
	}
}

ACMD_FUNC(advstats){
	nullpo_retr(-1, sd);

	char temp[100];
	char output[CHAT_SIZE_MAX];
	char* output_p = output;

	sprintf(output, " Basic Stats:");
	clif_displaymessage(fd, output);
	sprintf(output, "  Base ATK: %d, Equip ATK: %d, Weapon ATK (R/L): %d / %d\n  MATK: %d ~ %d", 
		sd->battle_status.batk, sd->battle_status.eatk, sd->battle_status.watk, sd->battle_status.watk2, sd->battle_status.matk_min, sd->battle_status.matk_max);
	clif_displaymessage(fd, output);
	sprintf(output, "  HP Bonus: +%d, +%d%%, SP Bonus: +%d, +%d%%", sd->bonus.hp, sd->hprate-100, sd->bonus.sp, sd->sprate-100);
	clif_displaymessage(fd, output);
	sprintf(output, "  DEF: %d + %d, MDEF: %d + %d (Hard + Soft Values)", sd->battle_status.def, sd->battle_status.def2, sd->battle_status.mdef, sd->battle_status.mdef2);
	clif_displaymessage(fd, output);
	sprintf(output, "  Hit: %d, P.Hit: %d\n  Flee: %d, P.Flee: %d", sd->battle_status.hit, sd->bonus.perfect_hit + sd->bonus.perfect_hit_add, sd->battle_status.flee, sd->battle_status.flee2/10);
	clif_displaymessage(fd, output);
	sprintf(output, "  Armor Element: %s %d", melement[sd->battle_status.def_ele], sd->battle_status.ele_lv);
	clif_displaymessage(fd, output);
	clif_displaymessage(fd, "========================================");

	weapon_atk* weapon;
	if (sd->battle_status.rhw.wlv > 0) {
		weapon = &sd->battle_status.rhw;
		clif_displaymessage(fd, " Right Weapon:");
		sprintf(output, "   wATK: %d, Refine ATK: %d, MATK: %d", weapon->atk, weapon->atk2, weapon->matk);
		clif_displaymessage(fd, output);
		sprintf(output, "   Range: %d, Weapon Level: %d", weapon->range, weapon->wlv);
		clif_displaymessage(fd, output);
		sprintf(output, "   Element: %s", melement[weapon->ele]);
		clif_displaymessage(fd, output);
	}

	if (sd->battle_status.lhw.wlv > 0) {
		weapon = &sd->battle_status.lhw;
		clif_displaymessage(fd, " Left Weapon:");
		sprintf(output, "   wATK: %d, Refine ATK: %d, MATK: %d", weapon->atk, weapon->atk2, weapon->matk);
		clif_displaymessage(fd, output);
		sprintf(output, "   Range: %d, Weapon Level: %d", weapon->range, weapon->wlv);
		clif_displaymessage(fd, output);
		sprintf(output, "   Element: %s", melement[weapon->ele]);
		clif_displaymessage(fd, output);
	}

	clif_displaymessage(fd, "========================================");
	adv_stats_sub_resistances(sd, fd);
	clif_displaymessage(fd, "========================================");
	adv_stats_sub_skill(sd,fd);
	clif_displaymessage(fd, "========================================");
	adv_stats_sub_critical(sd,fd);
	clif_displaymessage(fd, "========================================");
	adv_stats_sub_casting(sd,fd);
	clif_displaymessage(fd, "========================================");
	adv_stats_sub_attack(sd,fd);
	clif_displaymessage(fd, "========================================");
	adv_stats_sub_magical(sd,fd);

	return 0;
}



