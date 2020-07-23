local function OnLoad()
	ShowImage("GLOBAL/60010-advance_norm")
	ShowImage("GLOBAL/60000-backtrack_norm")
	ShowImage("GLOBAL/60030-id_norm")
	ShowImage("GLOBAL/60060-hkeep_norm")
	ShowImage("GLOBAL/60020-tv_norm")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "GLOBAL/60060-hkeep_norm", "GLOBAL/60061-hkeep_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/60000-backtrack_norm", "GLOBAL/60001-backtrack_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/60010-advance_norm", "GLOBAL/60011-advance_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/60020-tv_norm", "GLOBAL/60021-tv_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/60030-id_norm", "GLOBAL/60031-id_hi")
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/60060-hkeep_norm") then
		LoadScene("housekeeping2")
	end

	if IsOverWidget(x, y, "GLOBAL/60030-id_norm") then
		LoadScene("backpack")
	end
end

--------- --------- ---------

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "SCENE1/2000-Scene1.CLU",
		XPKS = {
			"GLOBAL/60012-advance_dim.XPK",
			"GLOBAL/60011-advance_hi.XPK",
			"GLOBAL/60010-advance_norm.XPK",
			"GLOBAL/60002-backtrack_dim.XPK",
			"GLOBAL/60001-backtrack_hi.XPK",
			"GLOBAL/60000-backtrack_norm.XPK",
			"GLOBAL/60050-camera1.XPK",
			"GLOBAL/60051-camera2.XPK",
			"GLOBAL/60031-id_hi.XPK",
			"GLOBAL/60030-id_norm.XPK",
			"GLOBAL/60062-hkeep_dim.XPK",
			"GLOBAL/60061-hkeep_hi.XPK",
			"GLOBAL/60060-hkeep_norm.XPK",
			"GLOBAL/60021-tv_hi.XPK",
			"GLOBAL/60020-tv_norm.XPK",
		},
	},
}
