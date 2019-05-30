function koder(text)
  local words = ""
  local ru = stringToArray('ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ')
	local sl = stringToArray('A€‹‚ƒEE„ˆ…†K‡–­OŒPCYX‰Š‘’“”•a—¢™šee›Ÿœk¯®o£pc¦y˜x ¤¥¡§¨©ª«¬')
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
