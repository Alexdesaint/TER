#include "HTMLgenerator.hpp"

#include <Arduino.h>

String header = R"=====(
<html>
<head>
    <title>Mesures de temps de vol</title>
    <meta charset="UTF-8">
    <style>
        body {
            width: 600px;
            margin-left: auto;
            margin-right: auto;
            margin-top: 0px;
        }

        @media screen and (max-width: 600px) {
            body {
                width: 100%;
            }
        }

        h1 {
            line-height: 2;
            font-size: 1em;
        }

        /* tableau général */
        table {
            border-collapse: collapse;
            text-align: center;
        }

        table tr {
            background-color: white;
        }

        table tr:nth-child(even) {
            background-color: #f7f7f7;
        }

        table th {
            color: white;
            padding: 10px;
        }

        table td {
            padding: 10px;
        }

        /* tableau de mesure de temps */

        .temps th {
            background-color: rgb(200, 50, 0);
            text-align: center;
        }

        .temps tr {
            background-color: white;
        }

        .temps tr:nth-child(even) {
            background-color: rgb(255, 220, 200);
        }

        .temps td {
            padding: 5px;
            border: 0px;
            text-align: center;
        }
    </style>

    <script>

        function line(c, x, y, a, b, epai, color) {
            c.beginPath();
            c.moveTo(x, 350 - y);
            c.lineTo(a, 350 - b);
            c.stroke();
        }

        function write(c, t, x, y) {
            c.fillText(t, x, 350 - y);
        }

        function writeVert(c, t, x, y) {
            c.save();
            c.rotate(-1.5707963267948966192313216916398);
            c.fillText(t, y - 350, x);
            c.restore();
        }

        function point(c, x, y, marge, taille, n, pas) {
            var l = marge + x * (taille / 9);
            var h = marge + y / pas * (taille / (n));

            c.beginPath();
            c.arc(l, 350 - h, 2, 0, 2 * Math.PI, false);
            c.fill();
            c.lineWidth = 0;
            c.fillStyle = c.strokeStyle;
            c.stroke();
        }

        function lineGraph(c, x, y, a, b, marge, taille, n, pas) {

            c.beginPath();
            c.fill();
            var l = marge + x * (taille / 9);
            var h = marge + y / pas * (taille / (n));

            c.moveTo(l, 350 - h);

            l = marge + a * (taille / 9);
            h = marge + b / pas * (taille / (n));

            c.lineTo(l, 350 - h);
            c.fill();
            c.stroke();
        }

        function drawGraph(e, dat) {

            var canvas = document.getElementById(e);
            var ctx = canvas.getContext("2d");
            ctx.font = "12px Arial";
            ctx.textAlign = "right";
            ctx.lineWidth = "1";
            ctx.strokeStyle = 'rgb(60, 60, 60)';

            div = 1000000;

            if(Math.max.apply(null, dat)/div < 1) {
                pas = 0.1;
            } else if(Math.max.apply(null, dat)/div < 2) {
                pas = 0.2;
            } else if(Math.max.apply(null, dat)/div < 3) {
                pas = 0.3;
            } else if(Math.max.apply(null, dat)/div < 4) {
                pas = 0.4;
            } 

            marge = 50;
            taille = 270;
            n = 10;

            ctx.textAlign = "right";

            for (var i = 0; i <= n; i++) {
                var l = marge + i * (taille / (n));
                line(ctx, marge - 5, l, taille + marge, l);
                write(ctx, (i * pas).toFixed(1), marge - 10, l - 5);
            }

            for (var i = 0; i < 10; i++) {
                var l = marge + i * (taille / 9);
                line(ctx, l, marge - 5, l, taille + marge);
                write(ctx, i + 1, l + 3, marge - 17);
            }

            ctx.textAlign = "center";
            write(ctx, "Saut", marge + taille / 2, marge - 30);
            writeVert(ctx, "Temps en s", marge - 30, marge + taille / 2);

            write(ctx, "Temps de vol", marge + taille / 4, marge + taille + 15);
            write(ctx, "Moyenne", marge + 3 * taille / 4, marge + taille + 15);

            ctx.strokeStyle = 'rgb(255, 50, 0)';
            ctx.lineWidth = "3";

            var sum = 0;
            for (var i = 0; i < dat.length; i++) {
                point(ctx, i, dat[i]/div, marge, taille, n, pas);
                if (i > 0) {
                    lineGraph(ctx, i - 1, dat[i - 1]/div, i, dat[i]/div, marge, taille, n, pas);
                }
                sum += dat[i];
            }

            ctx.fillRect(48, 4, 30, 12);

            ctx.strokeStyle = 'rgb(0, 0, 255)';
            sum /= 10;
            lineGraph(ctx, 0, sum / div, 9, sum / div, marge, taille, n, pas);

            ctx.fillStyle = ctx.strokeStyle;
            ctx.fillRect(195, 4, 30, 12);
        }
    </script>

    <script>

        function makeTableauDesTemps(table) {
            var ret = '<table class="temps"><tr><th>Saut</th><th>Temps de vol</th></tr>';

            var i;
            for (i = 0; i < 10; i++) {
                ret += '<tr><td>' + (i + 1) + '</td><td>' + (table[i] / 1000000).toFixed(3) + ' s</td></tr>';
            }

            var sum = 0;
            for (var j = 0; j < table.length; j++) {
                sum += table[j];
            }

            ret += '<tr><th>Total</th><td>' + (sum / 1000000).toFixed(3) + ' points </td></tr>';
            ret += '<tr><th>Moyenne</th><td>' + (sum / 1000000 / 10).toFixed(3) + ' s </td></tr>';
            ret += '</table>';

            return ret;
        }

        function makeTableauDesTempsExcel(table) {
            var ret = '<table border=\'2px\'>';
            ret += '<tr><th>Saut</th>'

            for (var i = 0; i < 10; i++) {
                ret += '<th>' + (i + 1) + '</th>';
            }

            ret += '<th>Total</th><th>Moyenne</th></tr>';

            for (var j = 0; j < table.length; j++) {
                ret += '<tr><th>Temps de vol ' + (j + 1) + '</th>';

                var sum = 0;
                for (var i = 0; i < 10; i++) {
                    ret += '<td>' + table[j][i] + '</td>';
                    sum += table[j][i];
                }

                ret += '<td>' + sum + '</td><td>' + sum / 10 + '</td></tr>';
            }
            ret += '</table>';

            return ret;
        }

        function fnExcelReport() {
            var tab_text = makeTableauDesTempsExcel(data);

            var ua = window.navigator.userAgent;
            var msie = ua.indexOf("MSIE ");

            if (msie > 0 || !!navigator.userAgent.match(/Trident.*rv\:11\./))      // If Internet Explorer
            {
                txtArea1.document.open("txt/html", "replace");
                txtArea1.document.write(tab_text);
                txtArea1.document.close();
                txtArea1.focus();
                sa = txtArea1.document.execCommand("SaveAs", true, "Say Thanks to Sumit.xls");
            }
            else                 //other browser not tested on IE 11
                sa = window.open('data:application/vnd.ms-excel,' + encodeURIComponent(tab_text));

            return (sa);
        }
    </script>
</head>

<body>
    <button id="btnExport" onclick="fnExcelReport();">Export to Excel</button>
    <div id="container"></div>
</body>

<script>
)=====";

String foot = R"=====(
    var code = '<table>';
    var i;
    for (i = 0; i < data.length; i++) {
        code += '<tr><td><h1>Tentative ' + (i + 1) + ' :</h1>' + makeTableauDesTemps(data[i]) + '</td><td><canvas id="chart' + i + '" width="350" height="350"></canvas></td></tr>';
    }

    code += '</table>';

    document.getElementById('container').insertAdjacentHTML('beforeend', code);

    for (var i = 0; i < data.length; i++) {

        var avg = 10;

        var sum = 0;
        for (var j = 0; j < data[i].length; j++) {
            sum += data[i][j];
        }

        avg = sum / 10;

        drawGraph('chart' + i, data[i]);
    }
</script>
</html>
)=====";

String page_vide = R"=====(
<!DOCTYPE html>
<html>

<head>
    <title>Mesures de temps de vol</title>
    <meta charset="UTF-8">

    <style>
        body {
            width: 600px;
            margin-left: auto;
            margin-right: auto;
            margin-top: 0px;
        }
    </style>
</head>

<body>
    <h2>Il n'y a pas de mesures</h2>
</body>

</html>
)=====";

// dataExample[] = "data=[ [10, 33, 55, 44, 10, 22, 10, 77, 55, 10], [44, 48, 30, 22, 55, 66, 45, 22, 66, 10], [10, 33, 55, 44, 10, 22, 10, 77, 55, 10], [44, 48, 30, 22, 55, 66, 45, 22, 66, 10]]";

String HTMLgenerator::getCode(std::list<std::array<uint32_t, 10>> temps) {
    if(temps.empty())
        return page_vide;

    String ret = header;

    String data = " var data=[ ";

    bool start1 = false, start2 = false;

    for (const std::array<uint32_t, 10> &tab : temps) {
        if (start1 == false)
            start1 = true;
        else 
            data += ", ";
            
        for (uint32_t t : tab) {
            if(start2 == false) {
                start2 = true;
                data += "[";
            } else
                data += ", ";
            

            data += std::uint32_t(t);
        }
        data += "]";
        start2 = false;
    }

    data += "]";

    ret += data;
    ret += foot;

    return ret;
}