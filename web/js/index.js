function gestioneGrafici(densita) {

// preparazione del vettore globale che contiene i dati da rappresentare nella canvas 
	stringaEventi = new Array;

// preaparazione delle canvas
	xCanvas = document.getElementById('graficoX');
	yCanvas = document.getElementById('graficoY');
	zCanvas = document.getElementById('graficoZ');
	ctx = xCanvas.getContext('2d');
	cty = yCanvas.getContext('2d');
	ctz = zCanvas.getContext('2d');

	ajaxGet(document.URL.slice(0,-1)+":8000",graph,densita);
}

function graph(content,densita) {

// acquisizione dei dati e aggiornamento del vettore stringaEventi.
	var nuoviEventi = content.split("\n");
	nuoviEventi.pop();
	var ultimoMarcatempo = (nuoviEventi[nuoviEventi.length-1].split(" "))[0]
	stringaEventi=nuoviEventi.concat(stringaEventi);

// rimozione dal vettore stringaEventi dei dati in eccesso
// (troppo lontani nel passato, quindi fuori dalla canvas).
	var l = stringaEventi.length;
		while ((ultimoMarcatempo - stringaEventi[l=l-1].split(" ")[0])/densita > ctx.canvas.width) stringaEventi.pop()

// pulizia e successivo ridisegno della canvas.
	ctx.clearRect(0,0,ctx.canvas.width,ctx.canvas.height);
	cty.clearRect(0,0,cty.canvas.width,cty.canvas.height);
	ctz.clearRect(0,0,ctz.canvas.width,ctz.canvas.height);

// traccia del primo accelerometro.
	ctx.strokeStyle="red";
	cty.strokeStyle="red";
	ctz.strokeStyle="red";

	ctx.beginPath();
	cty.beginPath();
	ctz.beginPath();
	for (var i=0;i<stringaEventi.length;i++) {
		var coord = stringaEventi[i].split(" ");
		if (coord.length==7) {
   		ctx.lineTo((ultimoMarcatempo - coord[0])/densita,50+Number(coord[1]));
   		cty.lineTo((ultimoMarcatempo - coord[0])/densita,50+Number(coord[2]));
   		ctz.lineTo((ultimoMarcatempo - coord[0])/densita,-245+50+Number(coord[3]));
		}
	}
	ctx.stroke();
	cty.stroke();
	ctz.stroke();

// traccia del secondo accelerometro.
	ctx.strokeStyle="#8ac007";
	cty.strokeStyle="#8ac007";
	ctz.strokeStyle="#8ac007";

	ctx.beginPath();
	cty.beginPath();
	ctz.beginPath();
	for (var i=0;i<stringaEventi.length;i++) {
		var coord = stringaEventi[i].split(" ");
		if (coord.length==7) {
   		ctx.lineTo((ultimoMarcatempo - coord[0])/densita,50+Number(coord[4]));
   		cty.lineTo((ultimoMarcatempo - coord[0])/densita,50+Number(coord[5]));
   		ctz.lineTo((ultimoMarcatempo - coord[0])/densita,-245+50+Number(coord[6]));
		}
	}
	ctx.stroke();
	cty.stroke();
	ctz.stroke();

// acquisizione ricorsiva di nuovi dati
	ajaxGet(document.URL.slice(0,-1)+":8000",graph,densita);
}
