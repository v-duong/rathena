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