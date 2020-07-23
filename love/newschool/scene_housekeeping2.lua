local function OnLoad()
	ShowImage("GLOBAL/63603-end_h")
	ShowImage("GLOBAL/63600-new_h")
	ShowImage("GLOBAL/63601-sel_h")
	ShowImage("GLOBAL/63710-vol1_h")
	ShowImage("GLOBAL/63711-vol2_h")
	ShowImage("GLOBAL/63712-vol3_h")
	ShowImage("GLOBAL/63713-vol4_h")
	ShowImage("GLOBAL/63714-vol5_h")
end

local function MouseMoved(x, y, dx, dy, istouch)
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/63603-end_h") then
		love.event.quit()
	end

	if IsOverWidget(x, y, "GLOBAL/63600-new_h") then
		LoadScene("scene1")
		--LoadScene("choice1")
	end
end

--------- --------- ---------

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "GLOBAL/63000-HKEEP.CLU",
		XPKS = {
			"GLOBAL/63603-end_h.XPK",
			"GLOBAL/63600-new_h.XPK",
			"GLOBAL/63601-sel_h.XPK",
			"GLOBAL/63710-vol1_h.XPK",
			"GLOBAL/63711-vol2_h.XPK",
			"GLOBAL/63712-vol3_h.XPK",
			"GLOBAL/63713-vol4_h.XPK",
			"GLOBAL/63714-vol5_h.XPK",
		},
	},
}
