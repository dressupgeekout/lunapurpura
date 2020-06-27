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

local function get_image_from_xpk(xpk, ...)
	local index = ... or 1
	local data = XPK.Decode(xpk, index)
	local entry = XPK.EntryAtIndex(xpk, index)
	return {
		x = entry.x,
		y = entry.y,
		image = love.graphics.newImage(love.image.newImageData(entry.width, entry.height, "rgba8", data)),
	}
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
	decoded_xpks = {}

	current_clu_index = 1
	current_xpk_index = 1

	table.insert(clus, CLU.NewFromFile("RESOURCE/!Terminal.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/D2Trailer.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/D3trailer.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/SCE.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_CDLogo.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_HKEEP.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_IDGlobal.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Scene1.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Scene2.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Scene3.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Scene4.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Scene5.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Scene6.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_Title.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_credits.CLU"))
	table.insert(clus, CLU.NewFromFile("RESOURCE/_pm_splash.CLU"))

	local clu_hkeep = clus[6]

	table.insert(xpks, XPK.NewFromFile("RESOURCE/_vol1_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_vol2_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_vol3_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_vol4_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_vol5_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_new_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_sel_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_end_h.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_hkeep_norm.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_tv_norm.XPK", clu_hkeep))
	table.insert(xpks, XPK.NewFromFile("RESOURCE/_id_norm.XPK", clu_hkeep))

	for _, xpk in ipairs(xpks) do
		table.insert(decoded_xpks, get_image_from_xpk(xpk))
	end
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

local function draw_clus()
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
end

function love.draw()
	for _, xpk in ipairs(decoded_xpks) do
		love.graphics.draw(xpk.image, xpk.x, xpk.y)
	end
end
