#include "track.h"

#include <QColor>
#include <qmath.h>


Track::Track(b2World *world):
    b2world(world) {
    generate();
}

Track::~Track() {
    for (b2Fixture *f = trackBody->GetFixtureList(); f; f = f->GetNext()) {
        if (f->GetUserData())
            delete (QColor *)f->GetUserData();
    }
    b2world->DestroyBody(trackBody);
}

//public

b2Body *Track::getBody() {
    return trackBody;
}

//private

void Track::generate() {
    b2BodyDef bodyDef;
    bodyDef.position.Set(LENGTH/2,0);
    bodyDef.type = b2_staticBody;
    trackBody = b2world->CreateBody(&bodyDef);
    b2PolygonShape polygonShape;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.friction = 10;
    fixtureDef.restitution = 0;
    fixtureDef.density = 0;
    fixtureDef.filter.categoryBits = 3;
    static float STATIC_TRACK[500] = {0, 0, 0, 0.0655485588586442, -0.230442155814722, 0.0336798058685298, -0.301751101631373, -0.368271509397048, 0.228385044794819, 0.225661340582431, -0.0690443906416994, -0.424661916442193, -0.331316767288022, -0.401343344438432, 0.281961127855537, -0.338755246749257, -0.25327469434599, 0.188555557996287, -0.129770143169596, -0.305850116187185, 0.242225442788169, 0.297554861299199, 0.229309900908578, -0.167074797993452, 0.316271873321031, -0.294657573346516, 0.122805781523265, 0.190504326104162, -0.110603753056473, 0.216114741592387, -0.132488194199045, 0.307074674962239, 0.243437562159377, 0.315808492363744, 0.259319230578411, -0.060180331255836, -0.520009072533817, 0.0736044261254756, 0.427164691567258, -0.363914894495385, -0.245810100556211, 0.180974695233894, -0.225387229041953, -0.396584629371035, -0.573358018829306, -0.12991041282341, -0.0754734281592167, -0.0699299578721745, 0.204291530966208, 0.358089406007397, 0.302250177445027, -0.127205945242427, 0.00110054783214411, 0.080937317992506, 0.420427351055063, -0.233887813867395, 0.372912361438343, 0.132654879746442, -0.582554456393318, 0.421864687879795, -0.332985671311515, -0.195879408632259, -0.383457101209845, 0.239980829352307, -0.401020819208805, 0.451575236927133, -0.370853946924491, -0.334154505272419, -0.36242271425752, -0.0873287003730153, -0.469497603559706, 0.345821517375695, -0.682344965338841, 0.0599559103621016, 0.450831877092348, 0.163079605684452, 0.169835485016437, 0.436201105280779, -0.233846103996077, 0.0288385426668245, -0.464553407128158, 0.344978635435411, -0.130991923196884, 0.0832072672664675, -0.266883063685273, 0.594549176140176, -0.42086886923328, -0.475857901200998, -0.705695167708871, 0.263369592255025, 0.220584440096807, -0.591833383514056, -0.708259756421061, -0.180467170109157, 0.015339406936296, -0.699740127464096, -0.475086061185432, 0.0474623293306079, 0.0195354802312267, -0.144770366056525, -0.0678379335630886, 0.219284996578112, 0.569680147156419, -0.806199006613187, -0.579557757868786, -0.1294926379821, 0.381379583867998, -0.861972606475059, -0.451367506740742, -0.163620619175889, 0.180117533375112, 0.44530976964355, 0.656561112213498, 0.652582757120488, 0.642002624140803, -0.331551163122517, -0.465748273428655, -0.126514192959523, 0.184760550113931, -0.512170827907794, 0.314012762526583, -0.863881896726484, -0.776022624865807, 0.413294648777211, -0.765981470690443, -0.250224443691503, 0.242045272814751, -0.390356400608264, -0.612635221832626, 0.152933230747341, -0.0683718657493762, 0.0925266914431406, 0.52947517037907, 0.0464080936154006, 0.551267820156875, -0.572244844654278, -0.0742179194076505, 0.401955161874371, -0.103895155411723, 0.0351264231318382, 0.0422930087897093, -0.664648665683772, -0.862522691749979, -0.94939223435309, -0.823710401282382, 0.366243126158874, -0.175707923082184, -0.952455864316118, 0.0940850100851725, -0.781602460971172, -0.396734049567177, -0.811465911515918, 0.336761427847129, -0.0157757776503588, 0.797991602193523, -0.347963150192995, 0.602624384130438, -0.581550920926237, -0.940889778882303, -0.837209988006838, -0.591547246826465, -0.656901385649544, -0.672463730403311, 0.313410617855744, 0.441758462941477, -1.04679863118095, 0.590514455307815, -0.367518593574305, 0.77752033151173, -1.10370607658096, 0.00171372636067877, -1.07377934948705, 0.649524389201661, 0.237723831308572, -0.813132564387199, -0.474671266795951, 0.275444190365427, -0.299427253229931, -0.256456391301165, 0.770013201350882, -1.03900150679835, 0.378192636673391, -0.246628000700455, -0.338942573804209, -0.544452070827078, -0.633199690051638, -0.0120339328758909, 0.73741081204415, 0.908792530957498, -0.412132019301886, 0.603868667501345, -0.22651295265538, -0.877761267698661, 0.116570588980503, -0.356478983140817, -0.0336225612231935, -0.320946616615116, -0.532464248395522, 0.388537103991525, 0.855216174019548, 0.318368493994288, 0.000959970120603099, -0.789467732738071, -0.22394916875456, -1.03037833982315, -0.503116993998664, -0.247491630988247, 0.355413187434435, 0.98036913708733, 0.99076743996258, -0.858301116874939, -0.193284530152463, 0.385838700954323, 0.807764442983001, 0.853781362167687, 0.972952404042838, -0.0373383019678019, -1.06477090926809, -0.982595386400432, -0.436901128765334, -0.707641089546069, -0.86126805342085, 0.721083488200688, -0.0884269247838819, 1.04810971598872, 0.662871120302842, 0.76898628684124, 0.819164275210933, 0.258045848503101, -0.572623696511678, 0.95281149201357, -0.398162338801305, -0.086681684287273, 0.426928971028362, 0.647945089955529, 0.774823082250049, 0.8628039826817, 0.401189356956885, -0.0393740988318982, 0.412602401711231, -1.0628636758062, -1.18374031919159, -0.61006555435939, -0.149483167473136, 0.611127887065388, 0.0982138888630701, 0.610728112707259, -1.12385844170689, -1.09247791064063, 0.844354305208163, 0.730606677181121, -0.484413649585668, -0.505642126605327, 0.851788392046137, 0.596642408083283, -0.339274394996779, 0.470173598704184, -0.937649952298415, -1.11291832550672, 1.1502410062127, -0.0220740435451112, -0.369958101632259, 0.93074066933067, -1.07300451042265, 0.912004725392152, -1.13497994326154, 0.524078316105698, -1.32857866617335, 0.756156482475518, -0.523014316743292, -0.16081016996771, 0.717190490403853, 0.777542813622298, -0.485437674420575, 0.892546691661566, -0.807869606065089, 0.862372224098451, -0.615728165051983, -1.46861845866607, -0.666063461481475, 0.73019700958027, -0.423818699912839, 1.10886709420819, -0.114682173931807, 1.18846876149955, 0.475343261695084, 0.911802168458045, -0.593827164255277, 0.0350740546116049, -0.000713914304363232, 0.263562679736875, -1.13574565954901, -1.04205545033796, 0.212245668553972, -1.22162860527574, -0.942426432932732, -0.160416216907039, -0.889450089383488, 1.06219802091841, 0.627740940637553, 1.10044556474541, 0.946094560748294, -0.760670504903723, 1.0829306571178, -1.20115437818236, -0.828179592938474, -0.513969347622774, 0.188552503501699, -1.28157213705583, 0.996923373660875, 1.12537283725074, 0.690408550197953, -1.49390298591854, 0.709893343189441, 0.332196714586482, 0.742441152533726, -1.3123767151226, -0.367951106748582, 1.23536876475512, -1.14552203881288, -0.00849099547238646, 0.705766314406731, -1.68241280198179, 1.3169571261221, -1.65489209800201, -1.29634381815657, -1.03148512344402, 1.24329680378121, -0.635989611926082, -1.27868838762022, 1.11532593169993, -1.20563325175498, -1.7278555656649, -0.8706985775398, 1.24207469775879, 0.347350643784203, -0.649463005454203, 0.958156913555458, -0.708658203024586, 0.576416628693924, 0.988792316388554, -0.195255925701387, 0.687982243017575, 1.08149898728631, 0.57237762648207, -1.07926819181661, -1.67959817097471, -0.196743723277328, -0.433350470136452, -0.669398612991477, 1.02260359233511, 0.573179442255315, -0.66569452945993, -0.379474253342169, -0.111036022852383, -0.544343427723451, -0.815199521026128, -0.828062122469107, 0.677317317193406, 0.725753527959203, -0.337231785165812, 0.0835911574820932, 0.102154448733685, 0.32216400233949, 0.969410112971583, 0.759228414290583, -0.837894955060453, 0.54024398186199, -0.772150252241687, 1.23716315017152, -0.57243622501162, -1.43380140870016, 0.91233083800827, -1.14937133486991, -1.35009504419569, -1.67071058751039, 0.511898741469753, -0.328565760907917, 0.759417829412484, -0.564540288203758, 0.872948439250084, -1.61279199510293, 0.317260639515547, 0.639709956903897, -1.02023102655691, 0.712364959775598, -1.6708750784581, -1.56404818260909, 0.854653605081608, -0.178444360177803, -0.68430231434086, 1.09729252824829, -0.734320852469743, -1.06255413665873, 0.390885616409758, -0.884462145228439, 0.195737475674391, -1.31582543153837, -0.00502932659518311, 0.575567823289772, 0.824382374755359, -0.275725092297972, 0.739757071647006, -1.65036412177122, -0.0816423112671117, 1.26247218457999, 0.633573999403038, 0.59554108589953, -1.61580195507825, -0.984399833026277, 0.218832462070075, 0.914276471947733, -0.816694052501763, -0.845150529117835, -1.52180645276749, 0.448203179954747, -0.0164433152496252, -0.320598835005475, -1.04877327616657, 1.24655300484866, -0.283194172695151, -0.893704851939473, -1.19647892729822, -1.68605454138686, 1.27848648476627, -0.334237939525724, 0.511877702513396, 0.361160362412815, 0.670923662225613, 1.12061118849849, -1.55478730139382, -1.55165191913176, 0.330819758726487, 1.29721029859547, -0.632694973007947, -1.1557887713851, 0.92807128624992, -0.765415260099031, 0.851193950496308, -0.953779532364647, 0.126264371587947, -0.353939046210296, 0.541996973511529, -1.33225241640902, 0.877939263918765, 0.332517525803701, -0.194335149145748, -0.258325339202014, 0.52234647931597, 0.873407442612617, 0.920605101845371, 1.02645508189151, 0.725771371288049, -1.57704170299017, -0.0958699288004905, -0.629537254327489, 0.587827253982669, -1.19637325940873, -0.05644728434671, 1.16315554956989, -1.13408784702569, 1.26429652660636, -1.69037061267575, 0.262126139729526, -0.819875862400116, -0.85171074093699, -1.13453165932986, 0.286212245194898, 0.973896675074103, -0.0266237572770987, -1.42094486590315, 0.165850153614662, -0.162166705303341, -1.70625937731617, 1.05129275335702, -1.38926100908093, 0.673905288783927, -1.28984583577112, 0.813463160707065, 0.170198913778778, 0.684888525774233, 1.12295176414966, -0.115397207111613, -0.930615870801166, -0.801134222445985, -1.28651723775525, 0.893028974014559, 1.06359431070707, 1.10894920538608, -0.00959818793379655, 0.914907892962553, -0.659496589114837, 1.08338606339461, -0.597918201791835, 0.477952265677178, -1.48077787861069, 0.235015979980155, 0.501303970350209, 0.131863498074839, -0.144127431015159};
    float track_length = 2;
    float prevX = -LENGTH/2 - 3;
    float prevY = 0;
	static bool inited = 0;
	static float saveX[500], saveY[500], saveAngle[500];
    if(!inited) for(int i = 0; i < 500; i++) {
        float angle = STATIC_TRACK[i];
        float half_dx = track_length*qCos(angle);
        float x = prevX + half_dx;
        float half_dy = track_length*qSin(angle);
        float y = prevY + half_dy;
        // polygonShape.SetAsBox(track_length, TRACK_THICK,
                              // b2Vec2(x, y), angle);
		saveX[i] = x;
		saveY[i] = y;
		saveAngle[i] = angle;
        // b2Fixture *f = trackBody->CreateFixture(&fixtureDef);
        // f->SetUserData(new QColor(0, 0, 0));
        prevX = x + half_dx;
        prevY = y + half_dy;
    }
	for(int i = 499; i >= 0; i--)
	{
        polygonShape.SetAsBox(track_length, TRACK_THICK,
                              b2Vec2(saveX[i], saveY[i]), saveAngle[i]);
        b2Fixture *f = trackBody->CreateFixture(&fixtureDef);
        f->SetUserData(new QColor(0, 0, 0));
	}
    polygonShape.SetAsBox(10, TRACK_THICK, b2Vec2(-LENGTH/2 - 13, 0), 0);
    b2Fixture *f = trackBody->CreateFixture(&fixtureDef);
    f->SetUserData(new QColor(0, 0, 0));
}
