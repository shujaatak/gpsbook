#include "dialoglinkedition.h"
#include "ui_dialoglinkedition.h"
#include <QFileInfo>

DialogLinkEdition::DialogLinkEdition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLinkEdition)
{
    ui->setupUi(this);
    mimeType["ai"] = "application/postscript";
    mimeType["aif"] = "audio/x-aiff";
    mimeType["aifc"] = "audio/x-aiff";
    mimeType["aiff"] = "audio/x-aiff";
    mimeType["asc"] = "text/plain";
    mimeType["asp"] = "text/html";
    mimeType["atom"] = "application/atom+xml";
    mimeType["au"] = "audio/basic";
    mimeType["avi"] = "video/x-msvideo";
    mimeType["bcpio"] = "application/x-bcpio";
    mimeType["bin"] = "application/octet-stream";
    mimeType["bmp"] = "image/bmp";
    mimeType["cdf"] = "application/x-netcdf";
    mimeType["cgm"] = "image/cgm";
    mimeType["class"] = "application/octet-stream";
    mimeType["cpio"] = "application/x-cpio";
    mimeType["cpt"] = "application/mac-compactpro";
    mimeType["csh"] = "application/x-csh";
    mimeType["css"] = "text/css";
    mimeType["dcr"] = "application/x-director";
    mimeType["dif"] = "video/x-dv";
    mimeType["dir"] = "application/x-director";
    mimeType["djv"] = "image/vnd.djvu";
    mimeType["djvu"] = "image/vnd.djvu";
    mimeType["dll"] = "application/octet-stream";
    mimeType["dmg"] = "application/octet-stream";
    mimeType["dms"] = "application/octet-stream";
    mimeType["doc"] = "application/msword";
    mimeType["dtd"] = "application/xml-dtd";
    mimeType["dv"] = "video/x-dv";
    mimeType["dvi"] = "application/x-dvi";
    mimeType["dxr"] = "application/x-director";
    mimeType["eps"] = "application/postscript";
    mimeType["etx"] = "text/x-setext";
    mimeType["exe"] = "application/octet-stream";
    mimeType["ez"] = "application/andrew-inset";
    mimeType["gif"] = "image/gif";
    mimeType["gram"] = "application/srgs";
    mimeType["grxml"] = "application/srgs+xml";
    mimeType["gtar"] = "application/x-gtar";
    mimeType["hdf"] = "application/x-hdf";
    mimeType["hqx"] = "application/mac-binhex40";
    mimeType["htm"] = "text/html";
    mimeType["html"] = "text/html";
    mimeType["ice"] = "x-conference/x-cooltalk";
    mimeType["ico"] = "image/x-icon";
    mimeType["ics"] = "text/calendar";
    mimeType["ief"] = "image/ief";
    mimeType["ifb"] = "text/calendar";
    mimeType["iges"] = "model/iges";
    mimeType["igs"] = "model/iges";
    mimeType["jnlp"] = "application/x-java-jnlp-file";
    mimeType["jp2"] = "image/jp2";
    mimeType["jpe"] = "image/jpeg";
    mimeType["jpeg"] = "image/jpeg";
    mimeType["jpg"] = "image/jpeg";
    mimeType["js"] = "application/x-javascript";
    mimeType["kar"] = "audio/midi";
    mimeType["latex"] = "application/x-latex";
    mimeType["lha"] = "application/octet-stream";
    mimeType["lzh"] = "application/octet-stream";
    mimeType["m3u"] = "audio/x-mpegurl";
    mimeType["m4a"] = "audio/mp4a-latm";
    mimeType["m4b"] = "audio/mp4a-latm";
    mimeType["m4p"] = "audio/mp4a-latm";
    mimeType["m4u"] = "video/vnd.mpegurl";
    mimeType["m4v"] = "video/x-m4v";
    mimeType["mac"] = "image/x-macpaint";
    mimeType["man"] = "application/x-troff-man";
    mimeType["mathml"] = "application/mathml+xml";
    mimeType["me"] = "application/x-troff-me";
    mimeType["mesh"] = "model/mesh";
    mimeType["mid"] = "audio/midi";
    mimeType["midi"] = "audio/midi";
    mimeType["mif"] = "application/vnd.mif";
    mimeType["mov"] = "video/quicktime";
    mimeType["movie"] = "video/x-sgi-movie";
    mimeType["mp2"] = "audio/mpeg";
    mimeType["mp3"] = "audio/mpeg";
    mimeType["mp4"] = "video/mp4";
    mimeType["mpe"] = "video/mpeg";
    mimeType["mpeg"] = "video/mpeg";
    mimeType["mpg"] = "video/mpeg";
    mimeType["mpga"] = "audio/mpeg";
    mimeType["ms"] = "application/x-troff-ms";
    mimeType["msh"] = "model/mesh";
    mimeType["mxu"] = "video/vnd.mpegurl";
    mimeType["nc"] = "application/x-netcdf";
    mimeType["oda"] = "application/oda";
    mimeType["ogg"] = "application/ogg";
    mimeType["pbm"] = "image/x-portable-bitmap";
    mimeType["pct"] = "image/pict";
    mimeType["pdb"] = "chemical/x-pdb";
    mimeType["pdf"] = "application/pdf";
    mimeType["pgm"] = "image/x-portable-graymap";
    mimeType["pgn"] = "application/x-chess-pgn";
    mimeType["php"] = "text/html";
    mimeType["pic"] = "image/pict";
    mimeType["pict"] = "image/pict";
    mimeType["png"] = "image/png";
    mimeType["pnm"] = "image/x-portable-anymap";
    mimeType["pnt"] = "image/x-macpaint";
    mimeType["pntg"] = "image/x-macpaint";
    mimeType["ppm"] = "image/x-portable-pixmap";
    mimeType["ppt"] = "application/vnd.ms-powerpoint";
    mimeType["ps"] = "application/postscript";
    mimeType["qt"] = "video/quicktime";
    mimeType["qti"] = "image/x-quicktime";
    mimeType["qtif"] = "image/x-quicktime";
    mimeType["ra"] = "audio/x-pn-realaudio";
    mimeType["ram"] = "audio/x-pn-realaudio";
    mimeType["ras"] = "image/x-cmu-raster";
    mimeType["rdf"] = "application/rdf+xml";
    mimeType["rgb"] = "image/x-rgb";
    mimeType["rm"] = "application/vnd.rn-realmedia";
    mimeType["roff"] = "application/x-troff";
    mimeType["rtf"] = "text/rtf";
    mimeType["rtx"] = "text/richtext";
    mimeType["sgm"] = "text/sgml";
    mimeType["sgml"] = "text/sgml";
    mimeType["sh"] = "application/x-sh";
    mimeType["shar"] = "application/x-shar";
    mimeType["silo"] = "model/mesh";
    mimeType["sit"] = "application/x-stuffit";
    mimeType["skd"] = "application/x-koan";
    mimeType["skm"] = "application/x-koan";
    mimeType["skp"] = "application/x-koan";
    mimeType["skt"] = "application/x-koan";
    mimeType["smi"] = "application/smil";
    mimeType["smil"] = "application/smil";
    mimeType["snd"] = "audio/basic";
    mimeType["so"] = "application/octet-stream";
    mimeType["spl"] = "application/x-futuresplash";
    mimeType["src"] = "application/x-wais-source";
    mimeType["sv4cpio"] = "application/x-sv4cpio";
    mimeType["sv4crc"] = "application/x-sv4crc";
    mimeType["svg"] = "image/svg+xml";
    mimeType["swf"] = "application/x-shockwave-flash";
    mimeType["t"] = "application/x-troff";
    mimeType["tar"] = "application/x-tar";
    mimeType["tcl"] = "application/x-tcl";
    mimeType["tex"] = "application/x-tex";
    mimeType["texi"] = "application/x-texinfo";
    mimeType["texinfo"] = "application/x-texinfo";
    mimeType["tif"] = "image/tiff";
    mimeType["tiff"] = "image/tiff";
    mimeType["tr"] = "application/x-troff";
    mimeType["tsv"] = "text/tab-separated-values";
    mimeType["txt"] = "text/plain";
    mimeType["ustar"] = "application/x-ustar";
    mimeType["vcd"] = "application/x-cdlink";
    mimeType["vrml"] = "model/vrml";
    mimeType["vxml"] = "application/voicexml+xml";
    mimeType["wav"] = "audio/x-wav";
    mimeType["wbmp"] = "image/vnd.wap.wbmp";
    mimeType["wbmxl"] = "application/vnd.wap.wbxml";
    mimeType["wml"] = "text/vnd.wap.wml";
    mimeType["wmlc"] = "application/vnd.wap.wmlc";
    mimeType["wmls"] = "text/vnd.wap.wmlscript";
    mimeType["wmlsc"] = "application/vnd.wap.wmlscriptc";
    mimeType["wrl"] = "model/vrml";
    mimeType["xbm"] = "image/x-xbitmap";
    mimeType["xht"] = "application/xhtml+xml";
    mimeType["xhtml"] = "application/xhtml+xml";
    mimeType["xls"] = "application/vnd.ms-excel";
    mimeType["xml"] = "application/xml";
    mimeType["xpm"] = "image/x-xpixmap";
    mimeType["xsl"] = "application/xml";
    mimeType["xslt"] = "application/xslt+xml";
    mimeType["xul"] = "application/vnd.mozilla.xul+xml";
    mimeType["xwd"] = "image/x-xwindowdump";
    mimeType["xyz"] = "chemical/x-xyz";
    mimeType["zip"] = "application/zip";

}

DialogLinkEdition::~DialogLinkEdition()
{
    delete ui;
}

void DialogLinkEdition::init(QString text, QString url, QString mimetype)
{
    ui->lineEditText->setText(text);
    ui->lineEditURL->setText(url);
    ui->lineEditMimeType->setText(mimetype);
}

void DialogLinkEdition::on_buttonBox_accepted()
{
    text = ui->lineEditText->text();
    url = ui->lineEditURL->text();
    mimetype = ui->lineEditMimeType->text();
}

void DialogLinkEdition::on_toolButtonAutoCompleteMimeType_clicked()
{
    //Find mimetype based on extension
    QFileInfo fileinfo(ui->lineEditURL->text());
    QString extension = fileinfo.suffix();
    if (mimeType.contains(extension))
    {
       ui->lineEditMimeType->setText(mimeType[extension]);
    }
    else
    {
        ui->lineEditMimeType->setText("text/html");
    }
}
