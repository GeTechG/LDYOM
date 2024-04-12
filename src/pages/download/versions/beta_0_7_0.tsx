import VersionInfo from "@site/src/pages/download/versionInfo";
import Link from "@docusaurus/Link";
import useBaseUrl from "@docusaurus/useBaseUrl";
import styles from "@site/src/pages/download/index.module.css";

function Beta_0_7_0() {
    return <VersionInfo version="Beta 0.7.0" date="28.07.20"
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
                        blogLink="2020/07/31/beta-0-7-0"/>;
}

export default Beta_0_7_0;