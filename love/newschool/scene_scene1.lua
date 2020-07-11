local function OnLoad()
	ShowImage("GLOBAL/_advance_norm")
	ShowImage("GLOBAL/_backtrack_norm")
	ShowImage("GLOBAL/_id_norm")
	ShowImage("GLOBAL/_hkeep_norm")
	ShowImage("GLOBAL/_tv_norm")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_hkeep_norm", "GLOBAL/_hkeep_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_backtrack_norm", "GLOBAL/_backtrack_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_advance_norm", "GLOBAL/_advance_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_tv_norm", "GLOBAL/_tv_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_id_norm", "GLOBAL/_id_hi")
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/_hkeep_norm") then
		LoadScene("housekeeping2")
	end

	if IsOverWidget(x, y, "GLOBAL/_id_norm") then
		LoadScene("backpack")
	end
end

--------- --------- ---------

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "SCENE1/_Scene1.CLU",
		XPKS = {
			"GLOBAL/_advance_dim.XPK",
			"GLOBAL/_advance_hi.XPK",
			"GLOBAL/_advance_norm.XPK",
			"GLOBAL/_backtrack_dim.XPK",
			"GLOBAL/_backtrack_hi.XPK",
			"GLOBAL/_backtrack_norm.XPK",
			"GLOBAL/_camera1.XPK",
			"GLOBAL/_camera2.XPK",
			"GLOBAL/_id_hi.XPK",
			"GLOBAL/_id_norm.XPK",
			"GLOBAL/_hkeep_dim.XPK",
			"GLOBAL/_hkeep_hi.XPK",
			"GLOBAL/_hkeep_norm.XPK",
			"GLOBAL/_tv_hi.XPK",
			"GLOBAL/_tv_norm.XPK",
		},
	},
}
