using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;

namespace Ejercicio_guia
{
    public partial class Form1 : Form
    {
        Socket server;
        public Form1()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //creamos un IPEndPoint con el ip del servidor y puerto del servidor
            //al que queremos conectarnos

            IPAddress direc = IPAddress.Parse("192.168.0.19");
            IPEndPoint ipep = new IPEndPoint(direc, 9050);

            //Creamos el socket

            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectarnos al socket
                this.BackColor = Color.Green;

                if (Longitud.Checked)
                {
                    string mensaje = "1/" + nombre.Text;
                    //Enviamos al servidor el nombre 
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show("La longitud de tu nombre es: " + mensaje);
                }
                else if (Bonito.Checked)
                {
                    string mensaje = "2/" + nombre.Text;
                    //Enviamos al servidor el nombre 
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    if(mensaje == "SI")
                    {
                        MessageBox.Show("Tu nombre es bonito");
                    }
                    else if(mensaje == "NO")
                    {
                        MessageBox.Show("Tu nombre no es bonito");
                    }
                }
                //Se termino el servicio 
                this.BackColor = Color.White;
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
            catch
            {
                MessageBox.Show("Error al conectar con el servidor");
            }
        }
    }
}
