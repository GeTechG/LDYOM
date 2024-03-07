import Layout from "@theme/Layout";
import styles from './index.module.css';
import Link from "@docusaurus/Link";
import useBaseUrl from "@docusaurus/useBaseUrl";
import {JSX} from "react";

type VersionInfoProps = {
    version: string,
    date: string,
    changes: JSX.Element,
    supportOld: boolean,
    buttons: JSX.Element[],
    blogLink: string,
    updatedDate?: string
}

const VersionInfo = ({version, date, changes, supportOld, buttons, blogLink, updatedDate}: VersionInfoProps) => {
    return (
        <div className={styles.version}>
            <div id="#version">
                <Link to={"blog/" + blogLink}><h2>LDYOM | {version}</h2></Link>
                <time dateTime={convertToDateTime(date)}>{date}</time>
                {
                    updatedDate ? <span><br/>Updated on <time dateTime={updatedDate}>{updatedDate}</time></span> : null
                }
            </div>
            <hr/>
            <div id="changelog">
                <p>Changes:</p>
                {changes}
                Support for missions from the previous version: <span
                className="positive_text">{supportOld ? "Yes" : "No"}</span>
            </div>
            <div className={styles.download_buttons}>
                {buttons}
            </div>
        </div>
    )
}

function convertToDateTime(dateString: string) {
    const parts = dateString.split('.');

    const day = Number(parts[0]);
    const month = Number(parts[1]) - 1;
    const year = Number('20' + parts[2]);

    const date = new Date(year, month, day);

    return date.toISOString().split('T')[0];
}

export default function Downloads(): JSX.Element {
    return (
        <Layout
            description="Create your world">
            <div>
                <VersionInfo version="Beta 0.8.0 Preview 9" date="05.03.24"
                             changes={
                                 <ul>
                                     <li>Reinstated removed features: Lua engine and node editor.</li>
                                 </ul>
                             }
                             supportOld={false}
                             buttons={[
                                 <Link to="https://github.com/GeTechG/LDYOM/releases/tag/Beta_0.8_preview_9"
                                       className={styles.button}>Download</Link>
                             ]}
                             blogLink="2024/05/03/13-57-beta-0-8-0-preview-9"/>
                <VersionInfo version="Beta 0.7.2"
                             date="20.07.21"
                             changes={
                                 <ul>
                                     <li>New nodes for pedestrians, cars, etc.</li>
                                     <li>Visual effects</li>
                                     <li>New objectives</li>
                                     <li>Add-on LCAPI</li>
                                     <li>Fixed and added new <span
                                         style={{textDecoration: "line-through"}}>bugs</span> features
                                     </li>
                                     <li>Many minor changes</li>
                                 </ul>
                             }
                             supportOld={true}
                             buttons={[
                                 <Link target="_blank" to={useBaseUrl("/downloads/Beta0.7.2.7z")}
                                       className={styles.button} download>Download LDYOM</Link>,
                                 <Link target="_blank" to={useBaseUrl("/downloads/LCAPI_0.7.2.7z")}
                                       className={styles.button} download>Download LCAPI</Link>
                             ]}
                             blogLink="2021/07/20/beta-0-7-2"
                             updatedDate="07.08.21"
                />
                <VersionInfo version="Beta 0.7.1" date="01.05.21"
                             changes={
                                 <ul>
                                     <li>Ported to C++</li>
                                     <li>New rendering for the node system</li>
                                     <li>Added a large number of new nodes</li>
                                     <li>Added trains</li>
                                     <li>New parameters for cars</li>
                                     <li>New parameters for actors</li>
                                     <li>New Cyber theme</li>
                                     <li>Backward compatibility for future versions</li>
                                     <li>Fixed and added new bugs</li>
                                     <li>Many minor changes</li>
                                 </ul>
                             }
                             supportOld={false}
                             buttons={[
                                 <Link target="_blank" to={useBaseUrl("/downloads/Beta 0.7.1.7z")}
                                       className={styles.button}>Download</Link>
                             ]}
                             blogLink="2021/05/01/beta-0-7-1"/>
                <VersionInfo version="Beta 0.7.0" date="28.07.20"
                             changes={
                                 <ul>
                                     <li>New interface</li>
                                     <li>Fixed language support</li>
                                     <li>Added node editor</li>
                                     <li>Added storyline mode</li>
                                     <li>New way to select skins, cars, weapons</li>
                                     <li>Bug fixes</li>
                                 </ul>
                             }
                             supportOld={false}
                             buttons={[
                                 <Link target="_blank" to={useBaseUrl("/downloads/Beta 0.7.0.7z")}
                                       className={styles.button}
                                       download>Download</Link>
                             ]}
                             blogLink="2020/07/31/beta-0-7-0"/>
            </div>
        </Layout>
    );
}