# project-rrjeta-kompjuterike

📌Description

Ky projekt implementon një sistem komunikimi Client-Server në C++ duke përdorur protokollin TCP. Sistemi përfshin menaxhimin e skedarëve, kontroll të bazuar në role (Admin vs. User), dhe një server dytësor HTTP për monitorimin e statusit të serverit në kohë reale.
Këtu përfshihen funksionalitete si ngarkimi, fshirja, kërkimi dhe leximi i skedarëve, si dhe menaxhimi automatik i klientëve inaktivë.

✨Features

•	Multi-Client Support: Mund të pranojë deri në 6 klientë njëkohësisht duke përdorur funksionin select().
•	Role-Based Access Control (RBAC): Klienti i parë që lidhet bëhet automatikisht Admin (ka të drejta fshirje/ngarkimi). Klientët e tjerë marrin rolin User (vetëm lexim).
•	HTTP API Monitoring: Serveri ka një port të hapur HTTP (8080) që kthen një përgjigje në formatin JSON me statusin e klientëve aktivë dhe log-et e mesazheve.
•	File Management: Suporton kërkim, lexim, marrje informacioni, fshirje dhe ngarkim skedarëve përmes komandave.
•	Auto-Reconnect: Klienti tenton të lidhet automatikisht nëse serveri bie ose shkëputet (Timeout).
•	Inactivity Timeout: Serveri largon automatikisht klientët që janë inaktivë për më shumë se 300 sekonda.

📂Project Structure

/project-root
│── Server.cpp      
│── Client.cpp      
│── README.md       

🚀 How to Compile and Run

Ky projekt është dizajnuar për sistemet operative Windows dhe kërkon kompilatorin GCC (MinGW).
1.	Kompilimi i Serverit dhe Klientit: 

Hap terminalin në folderin e projektit dhe ekzekuto këto komanda për t'i kompiluar të dy skedarët duke i lidhur me librarinë ws2_32:

g++ Server.cpp -o Server.exe -lws2_32
g++ Client.cpp -o Client.exe -lws2_32

2.	Ekzekutimi: 

Fillimisht starto serverin, pastaj klientin (ose disa klientë).
Në një terminal starto serverin:

./Server.exe

Në një terminal tjetër starto klientin:

./Client.exe

⚙️ Configuration

Nëse dëshiron të testosh projektin në kompjuterin tënd lokal pa rrjet të jashtëm, ndrysho IP-në në Server.cpp dhe Client.cpp përpara se ta kompilosh:

const string server_ipAddres = "127.0.0.1"; // Ose IP-ja jote në rrjetin lokal
int tcp_port = 7777; 
int http_port = 8080;

📜 Commands

Këto janë komandat që mund të shkruhen nga dritarja e klientit

    /list               -> Liston skedarët
    /read <file>        -> Lexon një skedar
    /search <text>      -> Kërkon skedarë
    /info <file>        -> Info për skedar
    /upload <file>      -> Ngarkon skedar
    /delete <file>      -> Fshin skedar
    /download <file>    -> (Në zhvillim)
    /exit               -> Mbyll klientin

🔄 How It Works

    1.	Inicializimi: Serveri ndizet dhe fillon të dëgjojë në portin TCP 7777 dhe atë HTTP 8080.
    2.	Lidhja e Klientit: Kur një klient lidhet, serveri e regjistron IP-në e tij. I pari merr rolin [Admin].
    3.	Përpunimi i Kërkesave: Klienti dërgon komanda (psh. /list). Serveri e përpunon kërkesën përmes funksionit select(), lexon ose menaxhon file-at në sistem, dhe i kthen përgjigje. Për përdoruesit standardë ka një vonesë sigurie prej 1.5 sekondash (Sleep(1500)).
    4.	API: Një browser i jashtëm mund të vizitojë http://localhost:8080 dhe të shohë log-et JSON të serverit.

💻 Example Usage

Dritarja e Serverit:

SERVERI NE PUNE...
TCP Port: 7777 | HTTP Port: 8080
[SISTEMI] Admin i ri: 172.16.108.31

Dritarja e Klientit (Admin):

U lidhet me Serverin!
Komanda: /list
Serveri: Server.cpp | Client.cpp | README.md |

⚠️ Limitations

- Maksimumi 6 klientë
- Nuk ka enkriptim (komunikim TCP në tekst të thjeshtë)
- Funksionon vetëm në Windows
