local function OnLoad()
end

local function MouseMoved(x, y, dx, dy, istouch)
end

local function MousePressed(x, y, button, istouch, npresses)
	LoadScene("housekeeping2")
end

--------- --------- ---------

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "Terminal/!Terminal.CLU",
		XPKS = {
			"Terminal/!Background_2.XPK",
			"Terminal/Background_1.XPK",
			"Terminal/Box_left.XPK",
			"Terminal/Box_left_RH.XPK",
			"Terminal/Box_right.XPK",
			"Terminal/Box_right_RH.XPK",
			"Terminal/Marquee_1.XPK",
			"Terminal/Marquee_2.XPK",
			"Terminal/Marquee_3.XPK",
			"Terminal/Marquee_4.XPK",
			"Terminal/Marquee_5.XPK",
			"Terminal/Mavis.XPK",
			"Terminal/Mavis_RH.XPK",
			"Terminal/Mavis_text.XPK",
			"Terminal/Mavis_text_RH.XPK",
			"Terminal/Rockett.XPK",
			"Terminal/Rockett_RH.XPK",
			"Terminal/Rockett_text.XPK",
			"Terminal/Rockett_text_RH.XPK",
			"Terminal/Sharla.XPK",
			"Terminal/Sharla_RH.XPK",
			"Terminal/Sharla_text.XPK",
			"Terminal/Sharla_text_RH.XPK",
		},
	},
}
