/* Copyright(C) 2005,2006,2007 Salvatore Sanfilippo <antirez@gmail.com>
 * ref: http://antirez.com/articoli/spaghettiajax.html
 *  * All Rights Reserved. */

var ajax_req = null;

function ajaxOk(ajax_req) {
    if (ajax_req.readyState == 4 && ajax_req.status == 200) {
    		console.log(ajax_req.getAllResponseHeaders());
				return ajax_req.responseText;
    } else {
        return false;
    }
}

function CreateXmlHttpReq(handler) {
  var xmlhttp = null;
  try {
    xmlhttp = new XMLHttpRequest();
  } catch(e) {
    try {
        xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    } catch(e) {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
  }
  xmlhttp.onreadystatechange = handler
  return xmlhttp;
}

function ajaxGet(url,handler){
    var a = new Array("segnaposto");
    for (var j=2; j<arguments.length; j++) {
        a[a.length] = arguments[j];
    }
    var myhandler = function() {
        var content = ajaxOk(ajax_req);
        if (content != false) {
            a[0] = content;
            return handler.apply(this, a);
        }
    }
    var ajax_req = CreateXmlHttpReq(myhandler);
    a[a.lenght] = ajax_req
    ajax_req.open("GET",url);
    ajax_req.send(null);
}
