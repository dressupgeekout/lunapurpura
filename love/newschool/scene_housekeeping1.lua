local function OnLoad()
	ShowImage("GLOBAL/63301-list")
	ShowImage("GLOBAL/63700-ok_n")
	ShowImage("GLOBAL/63704-up_h")
	ShowImage("GLOBAL/63705-down_h")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "GLOBAL/63700-ok_n", "GLOBAL/63701-ok_h")
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/63701-ok_h") then
		LoadScene("housekeeping2")
	end
end

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "GLOBAL/63000-HKEEP.CLU",
		XPKS = {
			"GLOBAL/63301-list.XPK",
			"GLOBAL/63600-new_h.XPK",
			"GLOBAL/63700-ok_n.XPK",
			"GLOBAL/63701-ok_h.XPK",
			"GLOBAL/63704-up_h.XPK",
			"GLOBAL/63705-down_h.XPK",
		},
	},
}
