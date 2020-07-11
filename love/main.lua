VERSION = "0.0.0a"

local serpent = require("serpent")

local CLU = require("luaclu")
local PRD = require("luaprd")
local PRX = require("luaprx")
local XPK = require("luaxpk")

require("games")

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

	if mouseover_entry then
		love.graphics.setColor(1, 1, 1, 1)
		local r, g, b = CLU.ColorAtIndex(clus[current_clu_index], mouseover_entry+1)
		love.graphics.print(string.format("Entry %d (%d,%d,%d)", mouseover_entry, r, g, b), 300, 0)
	end
end

--------- --------- ---------

--[[XXX In reality we'd refer to it by its number, or something along those
lines.]]
local function LoadCLU(path)
	return CLU.NewFromFile(CurrentGame.."/RESOURCE/"..path)
end

--[[Returns a table which contains information you need in order to
love.graphics.draw() it.]]
local function DecodeXPK(xpk, ...)
	local index = ... or 1
	local data = XPK.Decode(xpk, index)
	local entry = XPK.EntryAtIndex(xpk, index)
	return {
		Entry = entry,
		Image = love.graphics.newImage(love.image.newImageData(entry.width, entry.height, "rgba8", data)),
		Visible = false,
	}
end

--[[Reads an XPK from disk and also decodes it.]]
local function LoadXPK(path, clu)
	print(string.format("** LOAD XPK %q", path))
	local xpk = XPK.NewFromFile(CurrentGame.."/RESOURCE/"..path, clu)
	return DecodeXPK(xpk)
end

function LoadSound(path)
	print(string.format("** LOAD AIF %q", path))
	--[[XXX XXX XXX HACK: add ".wav" at the end for now]]
	local source = love.audio.newSource(CurrentGame.."/RESOURCE/"..path..".wav", "static")
	source:setLooping(false)
	return source
end

--------- --------- ---------

function IsOverWidget(x, y, name)
	local entry = CURRENT_SCENE.XPKS[name..".XPK"].Entry
	local min_x = entry.x
	local max_x = entry.x + entry.width
	local min_y = entry.y
	local max_y = entry.y + entry.height
	return x > min_x and x < max_x and y > min_y and y < max_y
end

function IsOverRegion(x, y, region)
	local min_x = region.x
	local max_x = region.x + region.width
	local min_y = region.y
	local max_y = region.y + region.height
	return x > min_x and x < max_x and y > min_y and y < max_y
end

--[[Satisfies all the requirements for a region struct!]]
function RegionFor(name)
	return GetImage(name).Entry
end

function OnMouseIn(x, y, dx, dy, region, callback)
	local old_x = x - dx
	local old_y = y - dy
	if not IsOverRegion(old_x, old_y, region) and IsOverRegion(x, y, region) then
		callback()
	end
end

function OnMouseOut(x, y, dx, dy, region, callback)
	local old_x = x - dx
	local old_y = y - dy
	if not IsOverRegion(x, y, region) and IsOverRegion(old_x, old_y, region) then
		callback()
	end
end

function OnMouseInOut(x, y, dx, dy, name, callback_in, callback_out)
	local region = RegionFor(name)
	OnMouseIn(x, y, dx, dy, region, callback_in)
	OnMouseOut(x, y, dx, dy, region, callback_out)
end

function GetImage(name)
	return CURRENT_SCENE.XPKS[name..".XPK"]
end

function GetSound(name)
	return CURRENT_SCENE.SOUNDS[name..".AIF"]
end

function ShowImage(name)
	GetImage(name).Visible = true
end

function HideImage(name)
	GetImage(name).Visible = false
end

--[[Creates MouseIn and MouseOut event handlers which will show the picture
with the `highlighted_name` when the mouse hovers over it, and the picture with
the `normal_name` otherwise.]]
function InstallHighlightable(x, y, dx, dy, normal_name, highlighted_name)
	OnMouseInOut(x, y, dx, dy, normal_name, function()
		HideImage(normal_name)
		ShowImage(highlighted_name)
	end, function()
		HideImage(highlighted_name)
		ShowImage(normal_name)
	end)
end

function PlaySound(name)
	GetSound(name):play()
end

function LoopSound(name)
	GetSound(name):setLooping(true)
	GetSound(name):play()
end

function StopSound(name)
	GetSound(name):stop()
end

--[[Loading a scene implies unloading the current one. XXX is this true in
the original?]]
function LoadScene(name)
	print(string.format("** Loading scene %q", name))
	local this = require(CurrentGame.."/scene_"..name)

	CURRENT_SCENE = {}
	CURRENT_SCENE.Name = name
	CURRENT_SCENE.CLU = LoadCLU(this.Template.CLU)
	CURRENT_SCENE.XPKS = {}
	CURRENT_SCENE.SOUNDS = {}
	CURRENT_SCENE.FUNCS = {
		OnLoad = this.OnLoad or function() end,
		MouseMoved = this.MouseMoved or function() end,
		MousePressed = this.MousePressed or function() end,
		KeyPressed = this.KeyPressed or function() end,
	}

	for _, path in ipairs(this.Template.XPKS) do
		CURRENT_SCENE.XPKS[path] = LoadXPK(path, CURRENT_SCENE.CLU)
	end

	if this.Template.SOUNDS then
		for _, path in ipairs(this.Template.SOUNDS) do
			CURRENT_SCENE.SOUNDS[path] = LoadSound(path)
		end
	end

	CURRENT_SCENE.FUNCS.OnLoad()
end

--------- --------- ---------

function love.load(argv, unfiltered_argv)
	love.window.setMode(640, 480)
	print(string.format("*** Luna Purpura % s***", VERSION))

	print(string.format("CLU -> %s", serpent.line(CLU)))
	print(string.format("PRD -> %s", serpent.line(PRD)))
	print(string.format("PRX -> %s", serpent.line(PRX)))
	print(string.format("XPK -> %s", serpent.line(XPK)))

	love.graphics.setBackgroundColor(127/255, 23/255, 151/255) -- "certain purple"

	CurrentGame = table.remove(argv, 1) or ""

	if not Games[CurrentGame] then
		local msg = string.format("No such game %q.\n\nValid games: %s", CurrentGame, table.concat(ValidGameNames, ", "))
		error(msg)
	end

	print(string.format("LOADING GAME: %s", Games[CurrentGame]))
	love.window.setTitle(string.format("Luna Purpura: %s", Games[CurrentGame]))

	CURRENT_SCENE = nil

	LoadScene("housekeeping1")
end

function love.keypressed(key, scancode)
	if key == "escape" then
		love.event.quit()
	end

	CURRENT_SCENE.FUNCS.KeyPressed(key, scancode)

	if cluview then
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
end

function love.mousemoved(x, y, dx, dy, istouch)
	if cluview then
		local entry_size = 20
		if x >= entry_size and x <= 17*entry_size and y >= entry_size and y <= 17*entry_size then
			mouseover_entry = math.floor(((y-entry_size) / entry_size)) * 16
			mouseover_entry = mouseover_entry + ((x-entry_size) / entry_size)
		end
	end

	CURRENT_SCENE.FUNCS.MouseMoved(x, y, dx, dy, istouch)
end

function love.mousepressed(x, y, button, istouch, npresses)
	CURRENT_SCENE.FUNCS.MousePressed(x, y, button, istouch, npresses)
end

function love.draw()
	for _, xpk in pairs(CURRENT_SCENE.XPKS) do
		if xpk.Visible then
			love.graphics.draw(xpk.Image, xpk.Entry.x, xpk.Entry.y)
		end
	end
end
