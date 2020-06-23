function koder(text)
  local words = ""
  local ru = stringToArray('АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя')
	local sl = stringToArray('AЂ‹‚ѓEE„€…†K‡–­OЊPCЏYЃX‰ЌЋЉђ‘’“”•a—ў™љee›џњќkћЇ®oЈpc¦y�x ¤ҐЎ§Ё©Є«¬')
	words = text
  for l = 1,#ru do
		words = string.gsub(words, ru[l], sl[l])
	end
  return words
end

function stringToArray(str)
  local t = {}
  for i = 1, #str do
    t[i] = str:sub(i, i)
  end
  return t
end

return koder
