// funzioni di servizio. utilizzate soprattutto per il debug

/*
 * funzione di servizio per stampare il contenuto di un vettore, utile a scopo di debug. codice acquisito qui:
 * http://www.compago.it/manuali/33-programmazione/350-print-arrays-or-object-in-javascript.htmlhttp://www.compago.it/manuali/33-programmazione/350-print-arrays-or-object-in-javascript.html
 * esempio d'uso:
 * e.innerHTML = print_r(vettore);
*/

function print_r(o,level,max) {
  var output = "";
  if(!level) level = 0;
  var padding = "";
  for(var j=0;j<level+1;j++) padding += "    ";
  if(!max) max = 10;
  if(level==max) return padding + "Max level ["+level+"] reached\n";
  if(typeof(o) == 'object') { 
    for(var item in o) {
      var value = o[item];
      if(typeof(value) == 'object') { 
        output += padding + "[" + item + "] => Array\n";
        output += print_r(value,level+1,max);
      } else {
        output += padding + "[" + item + "] => \"" + value + "\"\n";
      }
    }
  } else { 
    output = "("+typeof(o)+") => "+o;
  }
  return output;
}

/* funzione elementare di servizio, utile a scopo di debug */

function putContent(content,elementId) {
    var e = document.getElementById(elementId);
    e.innerHTML = content;
}
