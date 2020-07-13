local function OnLoad()
	ShowBackgroundImage("Global/B_BASIC")

	ShowImage("Global/new_adv_ylw")
	ShowImage("Global/sel_adv_ylw")
	ShowImage("Global/save_ylw")
	ShowImage("Global/end_ylw")

	--ShowImage("Global/SOFT_LOUD_VO1") -- XXX use-after-free? 
	ShowImage("Global/SOFT_LOUD_VO2")
	ShowImage("Global/SOFT_LOUD_VO3")
	ShowImage("Global/SOFT_LOUD_VO4")
	ShowImage("Global/SOFT_LOUD_VO5")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "Global/new_adv_ylw", "Global/new_advntr_pink")
	InstallHighlightable(x, y, dx, dy, "Global/sel_adv_ylw", "Global/select_advntr_p")
	InstallHighlightable(x, y, dx, dy, "Global/save_ylw", "Global/save_my_place_p")
	InstallHighlightable(x, y, dx, dy, "Global/end_ylw", "Global/end_pink")
end

local function MousePressed(x, y, button, istouch, npresses)
end

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "Global/!housekeeping.CLU",
		XPKS = {
			--"Global/SOFT_LOUD_VO1.XPK", -- XXX use-after-free?
			"Global/SOFT_LOUD_VO2.XPK",
			"Global/SOFT_LOUD_VO3.XPK",
			"Global/SOFT_LOUD_VO4.XPK",
			"Global/SOFT_LOUD_VO5.XPK",
			"Global/end_pink.XPK",
			"Global/end_ylw.XPK",
			"Global/new_adv_ylw.XPK",
			"Global/new_advntr_pink.XPK",
			"Global/save_my_place_p.XPK",
			"Global/save_ylw.XPK",
			"Global/sel_adv_ylw.XPK",
			"Global/select_advntr_p.XPK",
		},
		Backgrounds = {
			"Global/B_BASIC.XPK",
		},
	},
}
