local CLU = require("luaclu")

local function OnLoad()
	for i = 1, 255, 1 do
		local r, g, b = CLU.ColorAtIndex(CURRENT_SCENE.CLU, i)
		print(string.format("%d\t%d,%d,%d", i, r, g, b))
	end
end

local function MouseMoved(x, y, dx, dy, istouch)
end

local function MousePressed(x, y, button, istouch, npresses)
end

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Archive = "GLOBAL",
	CLU = 1000,
	XPKS = {
		"Global/select_advntr_p.XPK",
	},
}
