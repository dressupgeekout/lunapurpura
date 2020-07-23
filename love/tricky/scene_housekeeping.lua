local function OnLoad()
	ShowImage("Global/63604-new_adv_ylw")
	ShowImage("Global/63605-sel_adv_ylw")
	ShowImage("Global/63606-save_ylw")
	ShowImage("Global/63607-end_ylw")

	ShowImage("Global/63710-SOFT_LOUD_VO1")
	ShowImage("Global/63711-SOFT_LOUD_VO2")
	ShowImage("Global/63712-SOFT_LOUD_VO3")
	ShowImage("Global/63713-SOFT_LOUD_VO4")
	ShowImage("Global/63714-SOFT_LOUD_VO5")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "Global/63604-new_adv_ylw", "Global/63600-new_advntr_pink")
	InstallHighlightable(x, y, dx, dy, "Global/63605-sel_adv_ylw", "Global/63601-select_advntr_p")
	InstallHighlightable(x, y, dx, dy, "Global/63606-save_ylw", "Global/63602-save_my_place_p")
	InstallHighlightable(x, y, dx, dy, "Global/63607-end_ylw", "Global/63603-end_pink")
end

local function MousePressed(x, y, button, istouch, npresses)
end

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "Global/63000-!housekeeping.CLU",
		XPKS = {
			"Global/63710-SOFT_LOUD_VO1.XPK",
			"Global/63711-SOFT_LOUD_VO2.XPK",
			"Global/63712-SOFT_LOUD_VO3.XPK",
			"Global/63713-SOFT_LOUD_VO4.XPK",
			"Global/63714-SOFT_LOUD_VO5.XPK",
			"Global/63603-end_pink.XPK",
			"Global/63607-end_ylw.XPK",
			"Global/63604-new_adv_ylw.XPK",
			"Global/63600-new_advntr_pink.XPK",
			"Global/63602-save_my_place_p.XPK",
			"Global/63606-save_ylw.XPK",
			"Global/63605-sel_adv_ylw.XPK",
			"Global/63601-select_advntr_p.XPK",
		},
	},
}
