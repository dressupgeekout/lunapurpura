local function OnLoad()
	ShowImage("GLOBAL/_pause_h")
	ShowImage("GLOBAL/_stop_h")
end

local function MouseMoved(x, y, dx, dy, istouch)
end

local function MousePressed(x, y, button, istouch, npresses)
	LoadScene("housekeeping2")
end

---------- ---------- ----------

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "GLOBAL/_HKEEP.CLU",
		XPKS = {
			"GLOBAL/_pause_h.XPK",
			"GLOBAL/_play_h.XPK",
			"GLOBAL/_play_n.XPK",
			"GLOBAL/_stop_h.XPK",
		},
	},
}
