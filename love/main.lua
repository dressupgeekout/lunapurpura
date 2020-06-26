VERSION = "0.0.0a"

local serpent = require("serpent")

local CLU = require("luaclu")
local PRD = require("luaprd")
local XPK = require("luaxpk")

--------- --------- ---------

local function dump_clu(clu)
	print("========")
	print(clu)
	print("========")
	for i = 1, 256, 1 do
		r, g, b = CLU.ColorAtIndex(clu, i)
		print(i, r, g, b)
	end
	print("========")
end

local function render_xpk(xpk, ...)
	local index = ... or 1
	local pic = XPK.EntryAtIndex(xpk, index)
	--[[XXX arbitrary color for now, until we're able to properly decode:]] 
	love.graphics.setColor(0.5, 0.5, 0.5)
	love.graphics.rectangle("fill", pic.x, pic.y, pic.width, pic.height)
end

--------- --------- ---------

function love.load()
	love.window.setMode(640, 480)
	love.window.setTitle("Luna Purpura")
	print(string.format("*** Luna Purpura % s***", VERSION))

	print(string.format("CLU -> %s", serpent.line(CLU)))
	print(string.format("PRD -> %s", serpent.line(PRD)))
	print(string.format("XPK -> %s", serpent.line(XPK)))

	love.graphics.setBackgroundColor(127/255, 23/255, 151/255) -- "certain purple"

	clus = {}
	xpks = {}

	current_clu_index = 1
	current_xpk_index = 1

	table.insert(clus, CLU.NewFromFile("!Terminal.CLU"))
	table.insert(clus, CLU.NewFromFile("D2Trailer.CLU"))
	table.insert(clus, CLU.NewFromFile("D3trailer.CLU"))
	table.insert(clus, CLU.NewFromFile("SCE.CLU"))
	table.insert(clus, CLU.NewFromFile("_CDLogo.CLU"))
	table.insert(clus, CLU.NewFromFile("_HKEEP.CLU"))
	table.insert(clus, CLU.NewFromFile("_IDGlobal.CLU"))
	table.insert(clus, CLU.NewFromFile("_Scene1.CLU"))
	table.insert(clus, CLU.NewFromFile("_Scene2.CLU"))
	table.insert(clus, CLU.NewFromFile("_Scene3.CLU"))
	table.insert(clus, CLU.NewFromFile("_Scene4.CLU"))
	table.insert(clus, CLU.NewFromFile("_Scene5.CLU"))
	table.insert(clus, CLU.NewFromFile("_Scene6.CLU"))
	table.insert(clus, CLU.NewFromFile("_Title.CLU"))
	table.insert(clus, CLU.NewFromFile("_credits.CLU"))
	table.insert(clus, CLU.NewFromFile("_pm_splash.CLU"))

	table.insert(xpks, XPK.NewFromFile("_vol1_h.XPK"))
	table.insert(xpks, XPK.NewFromFile("_vol2_h.XPK"))
	table.insert(xpks, XPK.NewFromFile("_vol3_h.XPK"))
	table.insert(xpks, XPK.NewFromFile("_vol4_h.XPK"))
	table.insert(xpks, XPK.NewFromFile("_vol5_h.XPK"))
	--table.insert(xpks, XPK.NewFromFile("_new_h.XPK"))
	--table.insert(xpks, XPK.NewFromFile("_sel_h.XPK"))
	--table.insert(xpks, XPK.NewFromFile("_end_h.XPK"))
end

function love.update(dt)
end

function love.keypressed(key, scancode)
	if key == "escape" then
		love.event.quit()
	end

	if key == "left" then
		if current_clu_index == 1 then
			current_clu_index = #clus
		else
			current_clu_index = current_clu_index - 1
		end
	end

	if key == "right" then
		if current_clu_index == #clus then
			current_clu_index = 1
		else
			current_clu_index = current_clu_index + 1
		end
	end

	if key == "d" then
		dump_clu(clus[current_clu_index])
	end
end

function love.mousemoved(x, y, dx, dy, istouch)
	local entry_size = 20
	if x >= entry_size and x <= 17*entry_size and y >= entry_size and y <= 17*entry_size then
		mouseover_entry = math.floor(((y-entry_size) / entry_size)) * 16
		mouseover_entry = mouseover_entry + ((x-entry_size) / entry_size)
	end
end

function love.draw()
	for row = 1, 16, 1 do
		for column = 1, 16, 1 do
			local r, g, b = CLU.ColorAtIndex(clus[current_clu_index], (row-1)*16+column)
			local entry_size = 20
			love.graphics.setColor(r/255, g/255, b/255, 1.0)
			love.graphics.rectangle("fill", column*entry_size, row*entry_size, entry_size, entry_size)
		end
	end

	love.graphics.setColor(1, 1, 1, 1)
	love.graphics.print(CLU.GetName(clus[current_clu_index]), 0, 0)

	if mouseover_entry then
		love.graphics.setColor(1, 1, 1, 1)
		local r, g, b = CLU.ColorAtIndex(clus[current_clu_index], mouseover_entry+1)
		love.graphics.print(string.format("Entry %d (%d,%d,%d)", mouseover_entry, r, g, b), 300, 0)
	end

	love.graphics.setColor(1, 1, 1, 1)
	love.graphics.rectangle("fill", love.graphics.getWidth() - 58 - 10, 10, 58, 25)

	for _, xpk in ipairs(xpks) do
		render_xpk(xpk)
	end
end
