Games = {
	dance = "Rockett's First Dance",
	forest = "Secret Paths in the Forest",
	newschool = "Rockett's New School",
	secret = "Rockett's Secret Invitation",
	tricky = "Rockett's Tricky Decision",
}

ValidGameNames = {}
for k, _ in pairs(Games) do
	table.insert(ValidGameNames, string.format("%q", k))
end
