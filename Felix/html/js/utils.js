  var originalFontSize = $('html').css('font-size');

  function resetFontSizes()
  {
	setFontSizeForElement('html', originalFontSize);
	setFontSizeForElement('th', originalFontSize);
	setFontSizeForElement('td', originalFontSize);
	setFontSizeForElement('div', originalFontSize);
	return false;
  }
  
  // Increase Font Size
  function setFontSizeForElement(e, newFontSize)
  {
	$(e).css('font-size', newFontSize);
	return false;
  };
  
  function getBiggerFontSize(e)
  {
	var currentFontSize = $(e).css('font-size');
	var currentFontSizeNum = parseFloat(currentFontSize, 10);
	var newFontSize = currentFontSizeNum*1.2;
	if(newFontSize < 8)
	{
		newFontSize = 8;
	}
	if (newFontSize > 40)
	{
		newFontSize = 40;
	}
	return newFontSize;
  }
  
  function normalizeFontSizes()
  {
	var currentFontSize = $('html').css('font-size');
	setFontSizeForElement('html', currentFontSize);
	setFontSizeForElement('th', currentFontSize);
	setFontSizeForElement('td', currentFontSize);
	setFontSizeForElement('div', currentFontSize);
	return false;
  };
  
  function increaseFont()
  {
	var newFontSize = getBiggerFontSize('html') ;
	setFontSizeForElement('html', newFontSize);
	setFontSizeForElement('th', newFontSize);
	setFontSizeForElement('td', newFontSize);
	setFontSizeForElement('div', newFontSize);
	return false;
  };
  // Decrease Font Size
  function decreaseFontForElement(e)
  {
  	var currentFontSize = $(e).css('font-size');
 	var currentFontSizeNum = parseFloat(currentFontSize, 10);
    var newFontSize = currentFontSizeNum*0.8;
	if(newFontSize > 3)
	{
		$(e).css('font-size', newFontSize);
	}
	return false;
  };
  function decreaseFont()
  {
	decreaseFontForElement('html');
	decreaseFontForElement('th');
	decreaseFontForElement('td');
	decreaseFontForElement('div');
	return false;
  };
