local function OnLoad()
	ShowImage("GLOBAL/_down_h")
	ShowImage("GLOBAL/_list")
	ShowImage("GLOBAL/_up_h")
	ShowImage("GLOBAL/_ok_n")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_ok_n", "GLOBAL/_ok_h")
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/_ok_h") then
		LoadScene("housekeeping2")
	end
end

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "GLOBAL/_HKEEP.CLU",
		XPKS = {
			"GLOBAL/_down_h.XPK",
			"GLOBAL/_list.XPK",
			"GLOBAL/_new_h.XPK",
			"GLOBAL/_ok_h.XPK",
			"GLOBAL/_ok_n.XPK",
			"GLOBAL/_up_h.XPK",
		},
	},
}
