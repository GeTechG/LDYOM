function koder(text)
  local words = ""
  local ru = stringToArray('�����Ũ����������������������������������������������������������')
	local sl = stringToArray('A����EE����K���O�PC�Y�X����������a����ee����k���o�pc�y�x����������')
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
