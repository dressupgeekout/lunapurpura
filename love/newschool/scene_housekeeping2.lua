local function OnLoad()
	ShowImage("GLOBAL/_end_h")
	ShowImage("GLOBAL/_new_h")
	ShowImage("GLOBAL/_sel_h")
	ShowImage("GLOBAL/_vol1_h")
	ShowImage("GLOBAL/_vol2_h")
	ShowImage("GLOBAL/_vol3_h")
	ShowImage("GLOBAL/_vol4_h")
	ShowImage("GLOBAL/_vol5_h")
end

local function MouseMoved(x, y, dx, dy, istouch)
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/_end_h") then
		love.event.quit()
	end

	if IsOverWidget(x, y, "GLOBAL/_new_h") then
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
		CLU = "GLOBAL/_HKEEP.CLU",
		XPKS = {
			"GLOBAL/_end_h.XPK",
			"GLOBAL/_new_h.XPK",
			"GLOBAL/_sel_h.XPK",
			"GLOBAL/_vol1_h.XPK",
			"GLOBAL/_vol2_h.XPK",
			"GLOBAL/_vol3_h.XPK",
			"GLOBAL/_vol4_h.XPK",
			"GLOBAL/_vol5_h.XPK",
		},
	},
}
