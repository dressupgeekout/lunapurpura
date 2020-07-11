local function OnLoad()
	ShowImage("GLOBAL/_hkeep_norm")
	ShowImage("GLOBAL/_rockett_norm")
	ShowImage("IDL/Rock/_idl_roc_11_run") --XXX
	LoopSound("IDBackpk/_idb_BackpackAm")
end

local function MouseMoved(x, y, dx, dy, istouch)
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_hkeep_norm", "GLOBAL/_hkeep_hi")
	InstallHighlightable(x, y, dx, dy, "GLOBAL/_rockett_norm", "GLOBAL/_rockett_hi")
end

local function MousePressed(x, y, button, istouch, npresses)
	if IsOverWidget(x, y, "GLOBAL/_hkeep_norm") then
		StopSound("IDBackpk/_idb_BackpackAm")
		LoadScene("housekeeping2")
	end

	if IsOverWidget(x, y, "GLOBAL/_rockett_norm") then
		StopSound("IDBackpk/_idb_BackpackAm")
		LoadScene("scene1")
	end
end

---------- ---------- ----------

return {
	OnLoad = OnLoad,
	MouseMoved = MouseMoved,
	MousePressed = MousePressed,

	Template = {
		CLU = "IDGlobal/_IDGlobal.CLU",
		XPKS = {
			"GLOBAL/_hkeep_hi.XPK",
			"GLOBAL/_hkeep_norm.XPK",
			"GLOBAL/_rockett_hi.XPK",
			"GLOBAL/_rockett_norm.XPK",
			"IDL/Rock/_idl_roc_11_run.XPK", --XXX
		},
		SOUNDS = {
			"IDBackpk/_idb_BackpackAm.AIF",
		},
	},
}
