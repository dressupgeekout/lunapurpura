local function OnLoad()
	ShowImage("Global/end_pink")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "Global/end_pink", "Global/end_ylw")
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
			"Global/end_pink.XPK",
			"Global/end_ylw.XPK",
		},
	},
}
