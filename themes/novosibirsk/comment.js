function getText(aname) {
        txt = '';
        if (window.getSelection && !window.opera) txt = window.getSelection();
        else if (document.getSelection) txt = document.getSelection();
        else if (document.selection) txt = document.selection.createRange().text;
        txt='[quote="' + aname + '"]' + txt + '[/quote]\n'
}

function insertQuote(){
    if (txt != '' && document.getElementById('edit-comment')) {
    	insertAtCaret(document.getElementById("edit-comment"), txt);
    }
}

function insertAtCaret(textObj,textV){    
    textV=textV.replace(/\s\[\?\]/g,'')
    if (textV==''||!textObj) return
    var ver=8
    if(document.all && !window.opera){
      if (textObj.createTextRange&&textObj.caretPos) {
          var caretPos=textObj.caretPos
          caretPos.text=textV
        }else textObj.value+= textV
        }else
        {var brows=navigator.userAgent.toString()
         var scrollTop, scrollLeft
         if (textObj.type=='textarea'&&textObj.scrollTop)
         {scrollTop=textObj.scrollTop;scrollLeft=textObj.scrollLeft}                
          if(brows.search(/opera\/?(\d*.\d*)/i)!=-1) ver=RegExp.$1 
          if(textObj.selectionStart>=0&&ver>=8){
          if(textObj.textLength != undefined) 
           {var selLength=textObj.textLength 
            var selStart=textObj.selectionStart
            var selEnd=textObj.selectionEnd 
            if (selEnd==1||selEnd==2)selEnd=selLength  
            var s1=(textObj.value).substring(0,selStart) 
            var s2=(textObj.value).substring(selEnd,selLength)
            textObj.value=s1+textV+s2
            textObj.setSelectionRange(selStart+textV.length,selStart+textV.length) 
            } 
            if (typeof scrollTop != 'undefined')
            {textObj.scrollTop=scrollTop;textObj.scrollLeft=scrollLeft}
           }else textObj.value+=textV
        }
}
